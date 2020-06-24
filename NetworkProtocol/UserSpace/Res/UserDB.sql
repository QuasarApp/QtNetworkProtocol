
CREATE TABLE IF NOT EXISTS Users (
    id VARCHAR(64) PRIMARY KEY NOT NULL,
    userName VARCHAR(100) NOT NULL UNIQUE,
    pubKey VARCHAR(256) NOT NULL
);

CREATE TABLE IF NOT EXISTS UserPermisions (
    userId VARCHAR(64) NOT NULL,
    objectTable VARCHAR(100) NOT NULL,
    objectId INTEGER NOT NULL,
    lvl INTEGER NOT NULL,

    FOREIGN KEY(userId) REFERENCES Users(id)
            ON UPDATE CASCADE
            ON DELETE CASCADE

);
ALTER TABLE UserPermisions ADD UNIQUE INDEX (userId, objectTable, objectId);