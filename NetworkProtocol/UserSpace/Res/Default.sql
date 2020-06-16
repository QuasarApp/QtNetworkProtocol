
CREATE TABLE IF NOT EXISTS users(
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    name VARCHAR(100) DEFAULT NULL,
    pass VARCHAR(32) DEFAULT NULL,
    gmail VARCHAR(64) DEFAULT NULL UNIQUE,
    lastOnline INTEGER NOT NULL DEFAULT 0,
    onlinetime INTEGER NOT NULL DEFAULT 0,
    points INTEGER NOT NULL DEFAULT 0,
    data BLOB DEFAULT NULL

);