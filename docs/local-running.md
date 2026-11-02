# 本地启动与连接排查（Windows / VS Code）

## 先认识三个操作

| 操作 | 用途 | 完成后能否直接打开网页 |
|---|---|---|
| build.ps1 | 把C源码编译成程序 | 不能，尚未启动服务 |
| run.ps1 -Action migrate / seed | 创建表 / 导入测试身份 | 不能，初始化结束即退出 |
| run.ps1 | 启动并保持HTTP服务 | 能，需保持终端运行 |

本服务不会随电脑或VS Code自动启动。浏览器里的旧标签页也不能启动后端。

## 正常启动

1. VS Code打开包含README.md、backend和scripts的项目根目录。
2. 打开PowerShell终端，执行：

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File scripts/run.ps1
```

3. 等待出现 `B1-1 local server: http://127.0.0.1:8080`，保持该终端开启。
4. 打开 http://127.0.0.1:8080/ 。默认只接受本机连接，不是公网网址，也不是GitHub Pages。
5. 操作完成后在服务终端按Ctrl+C；下次访问前再启动。

如果已经有一个终端成功启动服务，不要再开第二份服务。请检查现有地址和端口。

## 页面提示“无法访问此站点”

### 第一步：查看启动终端

- 没有服务终端、终端已关闭，或程序已退出：按上面的命令启动。
- 有报错：按下方对照表处理；保留完整错误文字。
- 服务仍运行：确认地址是英文冒号 `http://127.0.0.1:8080/`，且端口与.env的PORT一致。

### 第二步：在另一个PowerShell终端检查健康接口

```powershell
# 如果你改过端口，把8080替换为实际端口
Invoke-RestMethod -Uri 'http://127.0.0.1:8080/api/health' -TimeoutSec 5
```

正常返回status为ok、milestone为B1-1。这个接口不需要先选择测试身份。

| 结果 | 含义与下一步 |
|---|---|
| status=ok、milestone=B1-1 | 服务正常。检查浏览器地址，刷新；必要时换本机Edge/Chrome访问，排查浏览器代理或沙箱对本机地址的限制 |
| 连接被拒绝 | 该地址没有可访问的监听服务；重新检查终端、启动错误和端口 |
| 超时 | 检查地址、端口、代理/安全软件及程序是否卡住；不要直接关闭系统防火墙 |
| 返回别的网页或内容 | 可能访问了另一个占用此端口的程序，继续检查监听进程 |

### 第三步：检查端口，不要直接杀进程

```powershell
Get-NetTCPConnection -LocalPort 8080 -State Listen -ErrorAction SilentlyContinue |
    Select-Object LocalAddress, LocalPort, OwningProcess

Get-Process campus-server -ErrorAction SilentlyContinue |
    Select-Object Id, ProcessName, Path
```

第一条无输出且命令没有权限错误，通常说明当前没有监听。若有OwningProcess，可将其数字填入下面命令查看程序：

```powershell
# 12345仅为示例，替换为实际OwningProcess
Get-Process -Id 12345 | Select-Object Id, ProcessName, Path
```

若已是本项目，直接使用已有服务；需要重启时在它的原终端Ctrl+C。如果是其他程序，不要随意终止；将.env中的PORT改为8081，再启动并访问 http://127.0.0.1:8081/ 。

## 启动错误对照

| 错误或表现 | 处理 |
|---|---|
| 找不到scripts/run.ps1 | 当前目录不对；VS Code打开项目根目录，确认有scripts文件夹 |
| Copy .env.example to .env first | 在根目录执行 `if (!(Test-Path .env)) { Copy-Item .env.example .env }`，保留已有配置 |
| Run scripts/build.ps1 first | 缺少可执行程序。GitHub拉取的源码不包含build/，需先安装工具链并build，见README |
| Cannot start server (port may be occupied) | 检查端口和已有服务；如无占用，保留完整错误并检查本机网络访问限制 |
| Database initialization failed | 检查DB_PATH及目录权限，通过run脚本启动；不要为了修连接问题删除数据库 |
| Development auth is forbidden outside development | 本地测试使用APP_ENV=development和ENABLE_DEV_AUTH=1；生产必须为0，不能绕过保护 |
| 网页能打开，但当前身份返回401 | 网络已经正常；先导入seed，检查开发配置，并在联调页选择测试身份 |
| 普通用户访问管理权限返回403 | 预期行为，不是启动失败；工作者身份才应返回200 |

## 重启、修改和验收

- 只重启：Ctrl+C → run，不用再次migrate、seed或build。
- C代码变化：Ctrl+C → build → run；运行中的exe可能使重新编译失败。
- .env变化：Ctrl+C → run，配置只在启动时读取。
- 新同学首次使用：按README完成工具链、build、配置、migrate、seed和run。
- `python tests/smoke.py`会自己启动临时服务并在结束时停止，**不会留下8080日常服务**。

需要协助时提供：启动命令、启动终端的完整错误、浏览器地址、健康检查结果。不要发送.env全文或任何服务密钥。
