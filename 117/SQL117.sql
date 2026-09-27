
--!!!8. Перемещение файлов БД в другое физическое место


--1 узнаём логические имена файлов
USE Bublioteka;
SELECT name, physical_name, type_desc FROM sys.database_files;
GO

--2 Переводим бд в ОФФ
USE master;
GO
ALTER DATABASE Bublioteka SET OFFLINE WITH ROLLBACK IMMEDIATE;
GO

--3 меняем путь в метаданных
ALTER DATABASE Bublioteka
MODIFY FILE (NAME = 'Bublioteka_Primary',
             FILENAME = 'D:\NewLocation\Bublioteka.mdf');

ALTER DATABASE Bublioteka
MODIFY FILE (NAME = 'Bublioteka_Data1',
             FILENAME = 'D:\NewLocation\Bublioteka_Data1.ndf');

ALTER DATABASE Bublioteka
MODIFY FILE (NAME = 'Bublioteka_Data2',
             FILENAME = 'D:\NewLocation\Bublioteka_Data2.ndf');

ALTER DATABASE Bublioteka
MODIFY FILE (NAME = 'Bublioteka_Idx1',
             FILENAME = 'D:\NewLocation\Bublioteka_Idx1.ndf');

ALTER DATABASE Bublioteka
MODIFY FILE (NAME = 'Bublioteka_People',
             FILENAME = 'D:\NewLocation\People.ndf');

ALTER DATABASE Bublioteka
MODIFY FILE (NAME = 'Bublioteka_Archive',
             FILENAME = 'D:\NewLocation\Archive.ndf');

ALTER DATABASE Bublioteka
MODIFY FILE (NAME = 'Bublioteka_Log',
             FILENAME = 'D:\NewLocation\Bublioteka_Log.ldf');
GO

--4 ФИЗ перемещаем файлы через проводник / командную строку:
--    move "C:\SQLData\Bublioteka\*.*" "D:\NewLocation\"

--5 возвращаем бд в ОН
ALTER DATABASE Bublioteka SET ONLINE;
GO

--6 проверяем
SELECT name, physical_name FROM sys.database_files;
GO



--!!!9. Отсоединение и присоединение БД

--ОТСОЕДИНЕНИЕ----------------------------------------------------------
USE master;
GO
ALTER DATABASE Bublioteka SET SINGLE_USER WITH ROLLBACK IMMEDIATE;
GO

EXEC sp_detach_db @dbname = N'Bublioteka', @skipchecks = N'true';
GO

-- (файлы физически переносятся на другой сервер вручную)

--ПРИСОЕДИНЕНИЕ---------------------------------------------------------
--на другом сервере
CREATE DATABASE Bublioteka ON
    (FILENAME = 'D:\SQLData\Bublioteka.mdf'),
    (FILENAME = 'D:\SQLData\Bublioteka_Data1.ndf'),
    (FILENAME = 'D:\SQLData\Bublioteka_Data2.ndf'),
    (FILENAME = 'D:\SQLData\Bublioteka_Idx1.ndf'),
    (FILENAME = 'D:\SQLData\People.ndf'),
    (FILENAME = 'D:\SQLData\Archive.ndf'),
    (FILENAME = 'D:\SQLData\Bublioteka_Log.ldf')
FOR ATTACH;
GO

ALTER DATABASE Bublioteka SET MULTI_USER;
GO






--!!!10. Удаление файлов БД через xp_cmdshell

--вкл расширенные параметры
EXEC sp_configure 'show advanced options', 1;
RECONFIGURE;
GO

--вкл xp_cmdshell
EXEC sp_configure 'xp_cmdshell', 1;
RECONFIGURE;
GO

--переводим бд в ОФФ (файлы не должны использоваться!)
ALTER DATABASE Bublioteka SET OFFLINE WITH ROLLBACK IMMEDIATE;
GO

--удаляем файлы
EXEC xp_cmdshell 'del "D:\NewLocation\Bublioteka_Data1.ndf"';
EXEC xp_cmdshell 'del "D:\NewLocation\Archive.ndf"';
GO

--отключаем обратно
EXEC sp_configure 'xp_cmdshell', 0;
RECONFIGURE;
GO

EXEC sp_configure 'show advanced options', 0;
RECONFIGURE;
GO