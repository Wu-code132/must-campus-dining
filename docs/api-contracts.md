# 共享接口约定 v1（B1-1）

## 基础约定
同源 `/api`，UTF-8 JSON。ID是字符串，时间采用UTC ISO 8601。价格用整数分（priceCents，MOP），避免浮点金额。错误返回 `{"error":{"code":"FORBIDDEN","message":"Staff role required"}}`。
401=未认证，403=身份已知但权限不足，404=不存在，405=方法不支持；业务实现后再补400/409等。分页未来采用 `limit` 与 `offset`，不要各模块私自更名。

## 已实现
- `GET /api/health`：公开，返回 `{"status":"ok","milestone":"B1-1"}`。
- `GET /api/me`：返回 `{"id":"user-demo-001","role":"campus_user","campusVerified":true}`。role只允许campus_user/canteen_staff。
- `GET /api/admin/ping`：仅canteen_staff可用，作为后端权限验收接口。
- 其他API返回404，非GET返回405。以下业务契约不是已完成接口。

## 开发身份（吴宇萱负责）
只在APP_ENV=development、ENABLE_DEV_AUTH=1、本机127.0.0.1请求时接受 `X-Dev-User`，值只允许user-demo-001、user-demo-002、staff-demo-001。缺失/伪造ID返回401。角色来自SQLite，X-Role等输入无效。
这是开发适配器，不是安全的真实登录；正式认证待B1-3/B2-1。production+ENABLE_DEV_AUTH=1启动直接失败；production+0即使携带开发头仍401。
模块调用 `require_user` / `require_staff` 获取User对象，不直接解析测试头，也不信任请求正文中的userId。真实会话引入后保留这两个入口。

## 菜品契约（赵淑文实现，当前仅docs/fixtures样例）
- `GET /api/dishes?limit=20&offset=0`：已验证用户，返回items/total。
- `GET /api/dishes/{id}`：已验证用户，返回单个菜品，不存在404。
- 基本字段：id、nameZh、nameEn、canteenId、canteenName、priceCents、isAvailable、spiceLevel、flavourTags。
- 凭证模块仅依赖id/name/canteen信息，不直接写菜品表；菜品下架保留ID和历史关联。

## 有效评分契约（吴宇萱实现，赵淑文使用）
- `GET /api/dishes/{id}/rating-summary?from=...&to=...`：已验证用户；返回dishId、reviewCount、averageRating、windowFrom、windowTo。区间左闭右开；无评分averageRating=null。
- `GET /api/me/rating-history?limit=20&offset=0`：仅当前用户自己的评分历史，返回items/total；不能通过userId参数查询他人。
- 内部服务边界（非公开HTTP）：按时间窗读取有效评分事件，用于榜单及推荐；禁止为便利把带用户ID的评分流水公开。
- 只有经批准凭证成功生成的评价才有效；B1-1提供合成样例，不能冒充真实社区数据。

## 周安琪的反馈模块
仅依赖currentUser；Feedback保存owner_id，查询/修改时检查所属用户或工作者。普通用户不能传其他owner_id创建反馈；不需要等待菜品、凭证或推荐。菜品关联如需加入，使用可空dish_id。
拟定接口：POST /api/feedback、GET /api/feedback/mine、GET /api/feedback/{id}；工作者处理走 /api/admin/feedback/...。具体字段由反馈负责人补充PR，不在B1-1实现。
