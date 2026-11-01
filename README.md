# must-campus-dining
Campus Dining Review and Recommendation System / D3-Team8

当前交付：**B1-1工程底座**。已完成C后端、静态前端联调页、SQLite身份表迁移、三种开发身份、后端角色权限、共享接口约定、Windows脚本和VS Code任务。

尚未实现真实邮箱/短信登录、凭证业务、反馈、菜品和推荐、生产部署。这些属于后续任务。本服务只监听127.0.0.1，不可直接对外上线。

## 环境与技术栈
Windows x64、VS Code、Git、PowerShell。使用固定LLVM-MinGW 20261020中的Clang编译C11源码。SQLite 3.50.4和CivetWeb 1.16源码随仓库提供，不需要安装数据库服务或Node.js。Python 3仅用于自动测试，不参与运行。

## 首次启动
若使用本次交付压缩包，已附build/campus-server.exe：可先复制.env、执行下方seed和run体验；需要改代码时再安装工具链并build。源码仓库不会提交可执行程序。
用VS Code打开项目根目录，终端选PowerShell：

```powershell
# 首次下载编译器到本项目.tools，核对SHA256，不修改系统PATH
powershell -NoProfile -ExecutionPolicy Bypass -File scripts/setup-toolchain.ps1
# 仅首次无.env时复制，不要覆盖已有个人配置
Copy-Item .env.example .env
powershell -NoProfile -ExecutionPolicy Bypass -File scripts/build.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File scripts/run.ps1 -Action migrate
powershell -NoProfile -ExecutionPolicy Bypass -File scripts/run.ps1 -Action seed
powershell -NoProfile -ExecutionPolicy Bypass -File scripts/run.ps1
```

浏览器打开 http://127.0.0.1:8080 ，Ctrl+C停止。也可“终端→运行任务”选择B1-1的build/seed/run。Bypass仅作用于该进程，不修改系统策略。首次工具链下载约191MB，解压需时间；构建产物在build/。
已有同版LLVM-MinGW可向build.ps1传 `-CompilerPath '绝对路径/bin/clang.exe'`，不依赖电脑原有GCC。

## 验收
联调页选择身份，再点击接口按钮：未登录查身份401；用户A/B返回不同ID；普通用户点管理权限403；工作者200。

```powershell
python tests/smoke.py
```

测试自动使用独立临时数据库与端口，不需要手动启动服务，不改data目录。覆盖迁移、种子幂等、角色约束、身份/权限伪造、静态文件边界、重启持久化及生产防护。两位队友仍须在各自电脑按本说明启动，才能完成“三人均能启动”的人工验收。

## 配置
`.env`支持APP_ENV（development/production）、ENABLE_DEV_AUTH（0/1）、PORT（1024～65535）、DB_PATH。run脚本读取.env并创建数据库父目录。直接运行exe读取进程环境，且工作目录必须是项目根目录。
production+开发身份开关1拒绝启动；production+0忽略所有开发身份头并返回401。真实生产会话尚未实现。开发身份不是校园认证，不适合共享网络服务。
别提交.env、真实联系方式、凭证、数据库、build/、.tools/。默认数据库位于data/campus.db。

## 目录与分工
| 路径 | 用途/负责人 |
|---|---|
| backend/src、include | 入口、路由、数据库与公共接口，吴宇萱 |
| backend/modules/auth、vouchers、reviews | 账号、凭证、评价，吴宇萱 |
| backend/modules/feedback | 反馈，周安琪 |
| backend/modules/dining | 菜品、过敏原和推荐，赵淑文 |
| frontend/modules | 对应业务页面，各模块负责人 |
| frontend/shared | 公共界面，吴宇萱；当前只是联调页 |
| backend/migrations、seeds | SQL版本迁移及合成开发数据 |
| docs、tests、scripts、vendor | 契约/样例、测试、脚本、固定源码依赖 |

先共同阅读[接口约定](docs/api-contracts.md)和[协作规则](docs/development.md)。周安琪只依赖currentUser即可开发反馈；赵淑文按合成评分开发，不等待评价模块。docs/fixtures仅是合成契约样例，不是已完成HTTP接口。

## 排错与下一步
- 端口占用：改.env的PORT，重新运行。
- 401：先seed，确认development及开发身份开关，浏览器选身份。
- 数据库初始化失败：确认从项目根目录运行、DB_PATH父目录存在。
- 重建演示库：停止服务，备份并移走自己的data/campus.db，再migrate/seed；不要删除含真实数据的库。
- B1-2再完成正式布局/双语和部署；B1-3/B2-1实现真实邮箱、手机认证。用户角色始终由服务端读取，不接受前端role。

本地分支feature/b1-1-foundation，未推送远程。报告依据：Task2 §3.1、§5.6、§15；Task3 §2.2、§4.1～4.2。当前分工以最新用户确认与四周表为准。
依赖及许可见[THIRD_PARTY.md](THIRD_PARTY.md)。项目自身许可证尚待团队选择。
