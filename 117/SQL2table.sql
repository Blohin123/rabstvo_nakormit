USE Bublioteka;
GO


--книги
CREATE TABLE Books (
    BookID           INT PRIMARY KEY IDENTITY(1,1),
    Title            VARCHAR(150) NOT NULL,
    Author           VARCHAR(150) NOT NULL,
    Genre            VARCHAR(80)  NOT NULL,
    PublicationYear  SMALLINT     NOT NULL
        CHECK (PublicationYear BETWEEN 1000 AND YEAR(GETDATE())),
    ISBN             VARCHAR(20)  NOT NULL UNIQUE
        CHECK (ISBN LIKE '978-%' OR ISBN LIKE '979-%'),
    TotalCopies      INT          NOT NULL DEFAULT 0 CHECK (TotalCopies >= 0),
    AvailableCopies  INT          NOT NULL DEFAULT 0
        CHECK (AvailableCopies >= 0 AND AvailableCopies <= TotalCopies)
) ON FG_Bublioteka;
GO

CREATE NONCLUSTERED INDEX IX_Books_Author          ON Books(Author)          ON FG_Indexes;
CREATE NONCLUSTERED INDEX IX_Books_Genre           ON Books(Genre)           ON FG_Indexes;
CREATE NONCLUSTERED INDEX IX_Books_PublicationYear ON Books(PublicationYear) ON FG_Indexes;
GO


--читатели
CREATE TABLE Reader (
    ReaderID    INT PRIMARY KEY IDENTITY(1,1),
    FullName    VARCHAR(150) NOT NULL,
    PhoneNumber VARCHAR(20)  NOT NULL UNIQUE
        CHECK (PhoneNumber LIKE '+7%' AND PhoneNumber NOT LIKE '%[^0-9+]%'),
    Email       VARCHAR(120) NULL
        CHECK (Email IS NULL OR Email LIKE '%@%.%'),
    RegDate     DATE         NOT NULL DEFAULT GETDATE(),
    IsActive    BIT          NOT NULL DEFAULT 1
) ON FG_People;
GO

CREATE NONCLUSTERED INDEX IX_Reader_PhoneNumber ON Reader(PhoneNumber) ON FG_People;
CREATE NONCLUSTERED INDEX IX_Reader_Email       ON Reader(Email)       ON FG_People;
GO


--выдачи книг
CREATE TABLE Loans (
    LoanID     INT PRIMARY KEY IDENTITY(1,1),
    BookID     INT  NOT NULL,
    ReaderID   INT  NOT NULL,
    LoanDate   DATE NOT NULL DEFAULT GETDATE(),
    DueDate    DATE NOT NULL,
    ReturnDate DATE NULL,
    FineAmount MONEY NOT NULL DEFAULT 0 CHECK (FineAmount >= 0),
    CONSTRAINT FK_Loans_Books  FOREIGN KEY (BookID)   REFERENCES Books(BookID),
    CONSTRAINT FK_Loans_Reader FOREIGN KEY (ReaderID) REFERENCES Reader(ReaderID),
    CONSTRAINT CK_Loans_Dates  CHECK (DueDate >= LoanDate
                                  AND (ReturnDate IS NULL OR ReturnDate >= LoanDate))
) ON FG_Bublioteka;
GO

CREATE NONCLUSTERED INDEX IX_Loans_BookID   ON Loans(BookID)   ON FG_Indexes;
CREATE NONCLUSTERED INDEX IX_Loans_ReaderID ON Loans(ReaderID) ON FG_Indexes;

--фильтр только по невозвращённым книгам
CREATE NONCLUSTERED INDEX IX_Loans_Active
    ON Loans(DueDate, ReaderID)
    WHERE ReturnDate IS NULL
    ON FG_Indexes;
GO


--штрафы
CREATE TABLE FINES (
    FineID       INT PRIMARY KEY IDENTITY(1,1),
    LoanID       INT   NOT NULL,
    ReaderID     INT   NOT NULL,
    Amount       MONEY NOT NULL CHECK (Amount > 0),
    CalculatedAt DATETIME NOT NULL DEFAULT GETDATE(),
    IsPaid       BIT   NOT NULL DEFAULT 0,
    CONSTRAINT FK_Fines_Loan   FOREIGN KEY (LoanID)   REFERENCES Loans(LoanID),
    CONSTRAINT FK_Fines_Reader FOREIGN KEY (ReaderID) REFERENCES Reader(ReaderID)
) ON FG_Bublioteka;
GO

CREATE NONCLUSTERED INDEX IX_Fines_LoanID          ON FINES(LoanID)             ON FG_Indexes;
CREATE NONCLUSTERED INDEX IX_Fines_ReaderID_IsPaid ON FINES(ReaderID, IsPaid)   ON FG_Indexes;
GO


--архив (FG_Archive)
CREATE TABLE LoansArchive (
    LoanID     INT PRIMARY KEY,
    BookID     INT NOT NULL,
    ReaderID   INT NOT NULL,
    LoanDate   DATE NOT NULL,
    DueDate    DATE NOT NULL,
    ReturnDate DATE NULL,
    FineAmount MONEY NOT NULL DEFAULT 0
) ON FG_Archive;
GO

CREATE NONCLUSTERED INDEX IX_Archive_ReaderID ON LoansArchive(ReaderID) ON FG_Archive;
CREATE NONCLUSTERED INDEX IX_Archive_LoanDate ON LoansArchive(LoanDate) ON FG_Archive;
GO