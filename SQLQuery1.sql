CREATE DATABASE Bublioteka
ON PRIMARY (
    NAME = 'Bublioteka_Primery',
    FILENAME = 'C:\DATE\BubliotekaP\BubliotekaP.mdf',
    SIZE = 5MB,
    MAXSIZE = 100MB,
    FILEGROWTH = 5MB
),
--1
FILEGROUP FG_Archive (
    NAME = 'Archive1',
    FILENAME = 'C:\DATE\Archive\Archive1.ndf',
    SIZE = 3MB,
    MAXSIZE = 20MB,
    FILEGROWTH = 2MB
), (
	NAME = 'Archive2',
    FILENAME = 'C:\DATE\Archive\Archive2.ndf',
    SIZE = 3MB,
    MAXSIZE = 20MB,
    FILEGROWTH = 2MB
	),
--2
FILEGROUP FG_Bublioteka (
    NAME = 'Bublioteka1',
    FILENAME = 'C:\DATE\Bublioteka\Bublioteka1.ndf',
    SIZE = 3MB,
    MAXSIZE = 20MB,
    FILEGROWTH = 2MB
), (
	NAME = 'Bublioteka_2',
    FILENAME = 'C:\DATE\Bublioteka\Bublioteka2.ndf',
    SIZE = 3MB,
    MAXSIZE = 20MB,
    FILEGROWTH = 2MB
	),

--3
FILEGROUP FG_Indexes (
    NAME = 'Bublioteka_Indexes1',
    FILENAME = 'C:\DATE\Bublioteka_Idx\Bublioteka_Idx1.ndf',
    SIZE = 3MB,
    MAXSIZE = 20MB,
    FILEGROWTH = 2MB
), (
	NAME = 'Bublioteka_Indexes2',
    FILENAME = 'C:\DATE\Bublioteka_Idx\Bublioteka_Idx2.ndf',
    SIZE = 3MB,
    MAXSIZE = 20MB,
    FILEGROWTH = 2MB
	),

--4
FILEGROUP FG_People (
    NAME = 'People1',
    FILENAME = 'C:\DATE\People\People1.ndf',
    SIZE = 3MB,
    MAXSIZE = 20MB,
    FILEGROWTH = 2MB
), (
	NAME = 'People2',
    FILENAME = 'C:\DATE\People\People2.ndf',
    SIZE = 3MB,
    MAXSIZE = 20MB,
    FILEGROWTH = 2MB
	)

LOG ON (
    NAME = 'Bublioteka_Log',
    FILENAME = 'C:\DATE\Bublioteka_log\Bublioteka_log.ldf',
    SIZE = 2MB,
    MAXSIZE = 50MB,
    FILEGROWTH = 2MB
);