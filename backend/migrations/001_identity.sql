CREATE TABLE users (
 id TEXT PRIMARY KEY,
 role TEXT NOT NULL CHECK(role IN ('campus_user','canteen_staff')),
 campus_verified INTEGER NOT NULL DEFAULT 0 CHECK(campus_verified IN (0,1)),
 created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);
-- Private contact data and real authentication are introduced by B1-3/B2-1.
-- This migration contains no real email addresses or phone numbers.
