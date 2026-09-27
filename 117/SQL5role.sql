USE master;
GO


--логины на уровне сервера
CREATE LOGIN admin_lib     WITH PASSWORD = 'Adm1n#2025!';
CREATE LOGIN librarian     WITH PASSWORD = 'L1br@ry2025';
CREATE LOGIN reader_user   WITH PASSWORD = 'R3ad3r#2025';
CREATE LOGIN inventory     WITH PASSWORD = 'Inv3nt0ry!';
GO

USE Bublioteka;
GO


--пользователи БД
CREATE USER admin_lib   FOR LOGIN admin_lib;
CREATE USER librarian   FOR LOGIN librarian;
CREATE USER reader_user FOR LOGIN reader_user;
CREATE USER inventory   FOR LOGIN inventory;
GO


--роли бд
CREATE ROLE Admin;
CREATE ROLE Librarian;
CREATE ROLE ReaderRole;
CREATE ROLE InventoryManager;
GO

ALTER ROLE Admin ADD MEMBER admin_lib;
GO
ALTER ROLE db_owner ADD MEMBER Admin;   -- полный доступ
GO


--червяк выдачи и читатели, каталог только на чтение
GRANT SELECT, INSERT, UPDATE ON Loans  TO Librarian;
GRANT SELECT, INSERT, UPDATE ON Reader TO Librarian;
GRANT SELECT                 ON Books  TO Librarian;
GRANT EXECUTE ON usp_IssueBook  TO Librarian;
GRANT EXECUTE ON usp_ReturnBook TO Librarian;
DENY  DELETE ON Books  TO Librarian;
DENY  SELECT, INSERT, UPDATE, DELETE ON FINES        TO Librarian;
DENY  SELECT, INSERT, UPDATE, DELETE ON LoansArchive TO Librarian;
GO


--КОУбой каталог + свои выдачи
GRANT SELECT ON Books    TO ReaderRole;
GRANT SELECT ON vMyLoans TO ReaderRole;
DENY  SELECT, INSERT, UPDATE, DELETE ON Loans        TO ReaderRole;
DENY  SELECT, INSERT, UPDATE, DELETE ON Reader       TO ReaderRole;
DENY  SELECT, INSERT, UPDATE, DELETE ON FINES        TO ReaderRole;
DENY  SELECT, INSERT, UPDATE, DELETE ON LoansArchive TO ReaderRole;
GO


--МОГер только каталог книг
GRANT SELECT, INSERT, UPDATE, DELETE ON Books TO InventoryManager;
DENY  SELECT, INSERT, UPDATE, DELETE ON Loans        TO InventoryManager;
DENY  SELECT, INSERT, UPDATE, DELETE ON Reader       TO InventoryManager;
DENY  SELECT, INSERT, UPDATE, DELETE ON FINES        TO InventoryManager;
DENY  SELECT, INSERT, UPDATE, DELETE ON LoansArchive TO InventoryManager;
GO

-- +
ALTER ROLE Librarian        ADD MEMBER librarian;
ALTER ROLE ReaderRole       ADD MEMBER reader_user;
ALTER ROLE InventoryManager ADD MEMBER inventory;
GO