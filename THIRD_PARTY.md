# 固定依赖与来源
- CivetWeb v1.16，commit d7ba35bbb649209c66e582d5a0244ba988a15159，https://github.com/civetweb/civetweb/tree/v1.16 。原许可在vendor/civetweb/LICENSE.md；本次禁用SSL/CGI，仅本地开发HTTP。
- SQLite 3.50.4 amalgamation：https://www.sqlite.org/2025/sqlite-amalgamation-3500400.zip 。公有领域声明保留于源码头；https://www.sqlite.org/copyright.html 。归档SHA256：1D3049DD0F830A025A53105FC79FD2AB9431AEA99E137809D064D8EE8356B032。
- LLVM-MinGW 20261020 Windows x86_64：https://github.com/mstorsjo/llvm-mingw/releases/tag/20261020 。不提交仓库；setup脚本校验归档SHA256 FD5462B7F5D93B2FF981FCD3F9C7E427E47F3203C64EA91C27BCC444EEBD74CD；各组件许可在下载包中。

这是固定可复现版本，不声称最新。正式部署前评估依赖安全更新、HTTPS和真实认证。
