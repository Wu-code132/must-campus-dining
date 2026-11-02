"""Integration tests: Python 3 standard library only."""
import json, os, pathlib, socket, sqlite3, subprocess, tempfile, time, urllib.request, urllib.error
ROOT=pathlib.Path(__file__).resolve().parents[1]
EXE=ROOT/'build/campus-server.exe'
checks=[]
def check(name, condition):
    if not condition: raise AssertionError(name)
    checks.append(name)
def request(port,path,identity=None,method='GET',extra=None):
    headers={} if identity is None else {'X-Dev-User':identity}
    headers.update(extra or {})
    req=urllib.request.Request(f'http://127.0.0.1:{port}{path}',headers=headers,method=method)
    try:
        with urllib.request.urlopen(req,timeout=3) as r:return r.status,r.read()
    except urllib.error.HTTPError as e:return e.code,e.read()
def launch(env):
    p=subprocess.Popen([str(EXE)],cwd=ROOT,env=env,stdout=subprocess.DEVNULL,stderr=subprocess.PIPE)
    try:
        for _ in range(100):
            if p.poll() is not None:raise RuntimeError(p.stderr.read().decode())
            try:
                if request(env['PORT'],'/api/health')[0]==200:return p
            except (OSError,urllib.error.URLError):pass
            time.sleep(.05)
        raise RuntimeError('Server startup timed out')
    except BaseException:
        p.terminate();p.wait();raise
def stop(p):
    p.terminate();p.wait(timeout=10);p.stderr.close()
with tempfile.TemporaryDirectory(prefix='b1-',dir=ROOT/'build') as temp:
    with socket.socket() as sock:
        sock.bind(('127.0.0.1',0));port=sock.getsockname()[1]
    env=dict(os.environ,APP_ENV='development',ENABLE_DEV_AUTH='1',PORT=str(port),DB_PATH=str(pathlib.Path(temp)/'test.db'))
    for action in ['--migrate','--seed-dev','--seed-dev','--migrate']:
        subprocess.run([str(EXE),action],cwd=ROOT,env=env,check=True,stdout=subprocess.DEVNULL)
    with sqlite3.connect(env['DB_PATH']) as db:
        check('Migration version 1',db.execute('pragma user_version').fetchone()[0]==1)
        check('Seed idempotent: three users',db.execute('select count(*) from users').fetchone()[0]==3)
        try:db.execute("insert into users(id,role) values('bad','admin')")
        except sqlite3.IntegrityError:check('Invalid role blocked',True)
        else:raise AssertionError('Invalid role accepted')
    db.close()
    p=launch(env)
    try:
        check('Health',request(port,'/api/health')[0]==200)
        check('Frontend',b'B1-1' in request(port,'/')[1])
        check('JavaScript',request(port,'/shared/app.js')[0]==200)
        check('Anonymous 401',request(port,'/api/me')[0]==401)
        for identity,role in [('user-demo-001','campus_user'),('user-demo-002','campus_user'),('staff-demo-001','canteen_staff')]:
            status,body=request(port,'/api/me',identity);data=json.loads(body)
            check(identity,status==200 and data['id']==identity and data['role']==role and data['campusVerified'] is True)
        check('User forbidden',request(port,'/api/admin/ping','user-demo-001')[0]==403)
        check('Staff allowed',request(port,'/api/admin/ping','staff-demo-001')[0]==200)
        check('Role header cannot escalate',request(port,'/api/admin/ping','user-demo-001',extra={'X-Role':'canteen_staff'})[0]==403)
        check('Forged identity',request(port,'/api/me',"' OR 1=1 --")[0]==401)
        check('Unimplemented API 404',request(port,'/api/dishes','user-demo-001')[0]==404)
        check('Wrong method 405',request(port,'/api/me','user-demo-001','POST')[0]==405)
        check('Private database not served',request(port,'/data/campus.db')[0]==404)
        check('Configuration not served',request(port,'/.env')[0]==404)
        check('Error schema',json.loads(request(port,'/api/me')[1])['error']['code']=='UNAUTHENTICATED')
    finally:stop(p)
    p=launch(env)
    try:check('Persistence after restart',request(port,'/api/me','user-demo-002')[0]==200)
    finally:stop(p)
    env['APP_ENV']='production'
    r=subprocess.run([str(EXE)],cwd=ROOT,env=env,capture_output=True,timeout=10)
    check('Production refuses dev auth startup',r.returncode==2)
    env['ENABLE_DEV_AUTH']='0';p=launch(env)
    try:check('Production ignores dev header',request(port,'/api/me','staff-demo-001')[0]==401)
    finally:stop(p)
    r=subprocess.run([str(EXE),'--seed-dev'],cwd=ROOT,env=env,capture_output=True,timeout=10)
    check('Production refuses dev seed',r.returncode==2)
print(json.dumps({'passed':len(checks),'checks':checks},ensure_ascii=False,indent=2))
