
CREATE VIEW vAvailableBooks AS
SELECT BookID, Title, Author, Genre, PublicationYear, AvailableCopies
FROM Books
WHERE AvailableCopies > 0;
GO


CREATE VIEW vActiveLoans AS
SELECT l.LoanID, b.Title, r.FullName, l.LoanDate, l.DueDate
FROM Loans l
JOIN Books  b ON b.BookID  = l.BookID
JOIN Reader r ON r.ReaderID = l.ReaderID
WHERE l.ReturnDate IS NULL;
GO


CREATE VIEW vMyLoans AS
SELECT l.LoanID, b.Title, l.LoanDate, l.DueDate, l.ReturnDate, l.FineAmount
FROM Loans l
JOIN Books  b ON b.BookID  = l.BookID
JOIN Reader r ON r.ReaderID = l.ReaderID
WHERE r.Email = SUSER_SNAME() + '@library.local';
GO


CREATE PROCEDURE usp_IssueBook
    @BookID   INT,
    @ReaderID INT,
    @Days     INT = 14
AS
BEGIN
    SET NOCOUNT ON;
    IF (SELECT AvailableCopies FROM Books WHERE BookID = @BookID) <= 0
    BEGIN
        RAISERROR('Нет доступных экземпляров книги', 16, 1);
        RETURN;
    END;

    INSERT INTO Loans (BookID, ReaderID, LoanDate, DueDate)
    VALUES (@BookID, @ReaderID, CAST(GETDATE() AS DATE),
            DATEADD(DAY, @Days, CAST(GETDATE() AS DATE)));

    UPDATE Books SET AvailableCopies = AvailableCopies - 1 WHERE BookID = @BookID;
END;
GO


CREATE PROCEDURE usp_ReturnBook
    @LoanID INT
AS
BEGIN
    SET NOCOUNT ON;
    DECLARE @BookID INT, @DueDate DATE, @ReaderID INT, @DaysLate INT, @Fine MONEY;

    SELECT @BookID = BookID, @DueDate = DueDate, @ReaderID = ReaderID
    FROM Loans WHERE LoanID = @LoanID AND ReturnDate IS NULL;

    IF @BookID IS NULL
    BEGIN
        RAISERROR('Активная выдача с таким LoanID не найдена', 16, 1);
        RETURN;
    END;

    SET @DaysLate = DATEDIFF(DAY, @DueDate, CAST(GETDATE() AS DATE));
    SET @Fine = CASE WHEN @DaysLate > 0 THEN @DaysLate * 10.0 ELSE 0 END;

    UPDATE Loans
    SET ReturnDate = CAST(GETDATE() AS DATE),
        FineAmount = @Fine
    WHERE LoanID = @LoanID;

    UPDATE Books SET AvailableCopies = AvailableCopies + 1 WHERE BookID = @BookID;

    IF @Fine > 0
        INSERT INTO FINES (LoanID, ReaderID, Amount) VALUES (@LoanID, @ReaderID, @Fine);
END;
GO


CREATE PROCEDURE usp_ArchiveOldLoans
AS
BEGIN
    SET NOCOUNT ON;
    DECLARE @cutoff DATE = DATEADD(YEAR, -1, CAST(GETDATE() AS DATE));

    INSERT INTO LoansArchive (LoanID, BookID, ReaderID, LoanDate, DueDate, ReturnDate, FineAmount)
    SELECT LoanID, BookID, ReaderID, LoanDate, DueDate, ReturnDate, FineAmount
    FROM Loans
    WHERE ReturnDate IS NOT NULL AND ReturnDate < @cutoff;

    DELETE FROM FINES  WHERE LoanID IN (SELECT LoanID FROM Loans WHERE ReturnDate IS NOT NULL AND ReturnDate < @cutoff);
    DELETE FROM Loans  WHERE ReturnDate IS NOT NULL AND ReturnDate < @cutoff;
END;
GO