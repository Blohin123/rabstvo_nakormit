-- Замените MyDB, пути и время на свои.

-- ========== Сценарий 1: DELETE без WHERE в таблице Читатели ==========

-- 1. Подготовка
ALTER DATABASE MyDB SET RECOVERY FULL;
GO
BACKUP DATABASE MyDB TO DISK = 'C:\Backups\MyDB_Full.bak' WITH INIT;
GO
BACKUP DATABASE MyDB TO DISK = 'C:\Backups\MyDB_Diff.bak' WITH DIFFERENTIAL, INIT;
GO

-- 2. Зафиксировать время до сбоя
SELECT GETDATE() AS TimeBeforeDelete;
GO

-- 3. Имитация сбоя
USE MyDB;
DELETE FROM Читатели;
GO

-- 4. Tail-log
BACKUP LOG MyDB TO DISK = 'C:\Backups\MyDB_TailLog.trn' WITH NO_TRUNCATE, INIT;
GO

-- 5. Восстановление
RESTORE DATABASE MyDB FROM DISK = 'C:\Backups\MyDB_Full.bak' WITH REPLACE, NORECOVERY;
GO
RESTORE DATABASE MyDB FROM DISK = 'C:\Backups\MyDB_Diff.bak' WITH NORECOVERY;
GO
RESTORE LOG MyDB FROM DISK = 'C:\Backups\MyDB_TailLog.trn' WITH STOPAT = '2026-10-14 10:15:00', RECOVERY;
GO

-- 6. Проверка
SELECT COUNT(*) FROM Читатели;
GO


-- ========== Сценарий 2: Повреждение .mdf ==========

-- 1. Подготовка
BACKUP DATABASE MyDB TO DISK = 'C:\Backups\MyDB_Full.bak' WITH INIT;
GO

-- 2. Имитация сбоя: остановить SQL Server, переименовать/удалить .mdf, запустить SQL Server

-- 3. Перевести БД в EMERGENCY и снять tail-log
ALTER DATABASE MyDB SET EMERGENCY;
GO
BACKUP LOG MyDB TO DISK = 'C:\Backups\MyDB_TailLog_Disaster.trn' WITH NO_TRUNCATE, INIT;
GO

-- 4. Восстановление
RESTORE DATABASE MyDB FROM DISK = 'C:\Backups\MyDB_Full.bak' WITH REPLACE, NORECOVERY;
GO
-- Если был плановый лог:
-- RESTORE LOG MyDB FROM DISK = 'C:\Backups\MyDB_Log.trn' WITH NORECOVERY;
-- GO
RESTORE LOG MyDB FROM DISK = 'C:\Backups\MyDB_TailLog_Disaster.trn' WITH RECOVERY;
GO

-- 5. Проверка
SELECT name, state_desc FROM sys.databases WHERE name = 'MyDB';
GO
SELECT COUNT(*) FROM Читатели;
GO


-- ========== Сценарий 3: DROP TABLE Штрафы ==========

-- 1. Подготовка
BACKUP DATABASE MyDB TO DISK = 'C:\Backups\MyDB_Full_BeforeDrop.bak' WITH INIT;
GO

-- 2. Зафиксировать время до сбоя
SELECT GETDATE() AS TimeBeforeDrop;
GO

-- 3. Имитация сбоя
USE MyDB;
DROP TABLE Штрафы;
GO

-- 4. Tail-log
BACKUP LOG MyDB TO DISK = 'C:\Backups\MyDB_TailLog_DropTable.trn' WITH NO_TRUNCATE, INIT;
GO

-- 5. Восстановление
RESTORE DATABASE MyDB FROM DISK = 'C:\Backups\MyDB_Full_BeforeDrop.bak' WITH REPLACE, NORECOVERY;
GO
RESTORE LOG MyDB FROM DISK = 'C:\Backups\MyDB_TailLog_DropTable.trn' WITH STOPAT = '2026-10-14 11:00:00', RECOVERY;
GO

-- 6. Проверка
SELECT * FROM sys.tables WHERE name = 'Штрафы';
GO
SELECT COUNT(*) FROM Штрафы;
GO
