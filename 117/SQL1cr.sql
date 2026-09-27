CREATE DATABASE Bublioteka

ON PRIMARY (
    NAME     = 'Bublioteka_Primary',
    FILENAME = 'C:\SQLData\Bublioteka\Bublioteka.mdf',
    SIZE     = 20MB,
    MAXSIZE  = 150MB,
    FILEGROWTH = 10MB
),


FILEGROUP FG_Bublioteka (
    NAME     = 'Bublioteka_Data1',
    FILENAME = 'C:\SQLData\Bublioteka\Bublioteka_Data1.ndf',
    SIZE     = 10MB,
    MAXSIZE  = 80MB,
    FILEGROWTH = 5MB
),
(
    NAME     = 'Bublioteka_Data2',
    FILENAME = 'C:\SQLData\Bublioteka\Bublioteka_Data2.ndf',
    SIZE     = 10MB,
    MAXSIZE  = 80MB,
    FILEGROWTH = 5MB
),


FILEGROUP FG_Indexes (
    NAME     = 'Bublioteka_Idx1',
    FILENAME = 'C:\SQLData\Bublioteka_Idx\Bublioteka_Idx1.ndf',
    SIZE     = 5MB,
    MAXSIZE  = 50MB,
    FILEGROWTH = 5MB
),


FILEGROUP FG_People (
    NAME     = 'Bublioteka_People',
    FILENAME = 'C:\SQLData\Bublioteka_People\People.ndf',
    SIZE     = 5MB,
    MAXSIZE  = 40MB,
    FILEGROWTH = 5MB
),


FILEGROUP FG_Archive (
    NAME     = 'Bublioteka_Archive',
    FILENAME = 'C:\SQLData\Bublioteka_Archive\Archive.ndf',
    SIZE     = 5MB,
    MAXSIZE  = 60MB,
    FILEGROWTH = 5MB
)


LOG ON (
    NAME     = 'Bublioteka_Log',
    FILENAME = 'C:\SQLData\Bublioteka_Log\Bublioteka_Log.ldf',
    SIZE     = 10MB,
    MAXSIZE  = 100MB,
    FILEGROWTH = 10MB
);
GO