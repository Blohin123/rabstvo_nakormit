USE master;
DROP DATABASE Bublioteka;

EXEC sp_configure 'show advanced options', 1;
RECONFIGURE;

EXEC sp_configure 'xp_cmdshell', 1;
RECONFIGURE;

EXEC xp_cmdshell 'del "C:\DATE\BubliotekaP\BubliotekaP.mdf"';
GO

EXEC xp_cmdshell 'del "C:\DATE\Bublioteka\Bublioteka1.ndf"';
GO
EXEC xp_cmdshell 'del "C:\DATE\Bublioteka\Bublioteka2.ndf"';
GO

EXEC xp_cmdshell 'del "C:\DATE\Bublioteka_Idx\Bublioteka_Idx1.ndf"';
GO
EXEC xp_cmdshell 'del "C:\DATE\Bublioteka_Idx\Bublioteka_Idx2.ndf"';
GO

EXEC xp_cmdshell 'del "C:\DATE\Bublioteka_log\Bublioteka_log.ldf"';
GO

EXEC xp_cmdshell 'del "C:\DATE\People\People1.ndf"';
GO
EXEC xp_cmdshell 'del "C:\DATE\People\People2.ndf"';
GO

EXEC xp_cmdshell 'del "C:\DATE\People\People1.ndf"';
GO
EXEC xp_cmdshell 'del "C:\DATE\Archive\Archive2.ndf"';
GO