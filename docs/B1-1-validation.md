# B1-1本地验收记录

日期：2026-11-02（Asia/Shanghai）。仓库起点：b0738881381c40192de6226c43026489915d047d；本地分支feature/b1-1-foundation。未推送、未创建远程PR。

## 已验证
- Windows x64，LLVM-MinGW 20261020，Clang 23.1.4，C11；通过项目scripts/build.ps1成功编译链接。
- scripts/run.ps1 -Action migrate / seed成功；真实SQLite文件可初始化。
- tests/smoke.py：23项通过。覆盖空库迁移、重复执行迁移/种子、角色约束、HTTP健康、静态页面/JS、未登录401、三种身份、普通用户403、工作者200、伪造角色/身份、未知业务API404、错误方法405、私有文件不公开、错误格式、重启持久化、production禁止开发认证及种子。
- git diff --check通过；.env、数据库、可执行程序和工具链被gitignore排除。

## 尚需团队完成
- 周安琪和赵淑文各自在自己的Windows电脑按README启动。尚未声称已验证队友的机器。
- 共同确认docs/api-contracts.md的字段约定；业务负责人可通过PR修改并同步。
- B1-2及以后：正式双端导航/双语、真实邮箱手机登录、业务模块、HTTPS部署与完整隐私加密等。不属于本次已完成范围。

## 本机环境处理
原D:/mingw25/bin/gcc.exe无法启动；未修改其安装。使用独立下载的LLVM-MinGW工具链完成验证。setup脚本提供固定下载地址和SHA256核验，放在项目.tools中，不更改系统PATH。

压缩包附build/campus-server.exe，可先复制.env.example、执行seed和run体验；正式开发仍按README安装工具链并从源码构建。压缩包不含.git历史、个人.env、数据库或工具链。
