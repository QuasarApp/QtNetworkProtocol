-- This is base table of network member (clients)
-- all additional information must be conteins in the another tables with links to the NetworkMembers table.
CREATE TABLE IF NOT EXISTS NetworkMembers (
    id VARCHAR(32) PRIMARY KEY,
    authenticationData BLOB NOT NULL,
    trust INTEGER default 0
);

-- The MemberPermisions table contains link to anothe database object and links to users of the database with permission lvl.
CREATE TABLE IF NOT EXISTS MemberPermisions (
    memberId VARCHAR(32) NOT NULL,
    -- This field contatins a base64 implementation of a sha256 (requariment 44 bytes) hash code of a database address
    dbAddress VARCHAR(44) NOT NULL,
    lvl tinyint NOT NULL,

    FOREIGN KEY(memberId) REFERENCES NetworkMembers(id)
            ON UPDATE CASCADE
            ON DELETE CASCADE

);

CREATE UNIQUE INDEX IF NOT EXISTS MemberPermisionsIndex ON MemberPermisions(memberId, dbAddress);

-- The DefaultPermissions table contains addresses of tables. and default permisions for all network members
CREATE TABLE IF NOT EXISTS DefaultPermissions (
    dbAddress VARCHAR(44) NOT NULL UNIQUE,
    lvl tinyint NOT NULL,

    FOREIGN KEY(dbAddress) REFERENCES MemberPermisions(dbAddress)
            ON UPDATE CASCADE
            ON DELETE CASCADE
)

