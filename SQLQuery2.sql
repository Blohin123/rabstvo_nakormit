USE Bublioteka;

------------------------------------------------------------------------------------------------
CREATE TABLE Books (
    BooksID INT PRIMARY KEY IDENTITY(1, 1),
    Name_books VARCHAR(100) NOT NULL,
   Author_books VARCHAR(100) NOT NULL,
   Publication_date DATETIME NOT NULL,
    Genre VARCHAR(100) NOT NULL
) ON Bublioteka;

CREATE INDEX IX_Books_Author_books
ON Books (Author_books)
ON FG_Bublioteka;

CREATE INDEX IX_Books_Genre
ON Books (Genre)
ON FG_Bublioteka;

CREATE INDEX IX_Books_Publication_date
ON Books (Publication_date)
ON FG_Archive;


------------------------------------------------------------------------------------------------
CREATE TABLE Reader (
    ReaderID INT PRIMARY KEY IDENTITY(1, 1),
    NameReader VARCHAR(100) NOT NULL,
	Nomber_Iphone VARCHAR(20) NOT NULL CHECK (Nomber_Iphone LIKE '+7%' AND nomber_iphone NOT LIKE'%[^0-9+]%')
) ON FG_People;

CREATE INDEX IX_Reader_Nomber_Iphone
ON Reader (Nomber_Iphone)
ON FGpeople_Indexes;


------------------------------------------------------------------------------------------------
CREATE TABLE Loans(
	LoanID INT PRIMARY KEY IDENTITY(1, 1),
	BooksID INT NOT NULL FOREIGN KEY REFERENCES Books(BookID),
	ReaderID INT NOT NULL FOREIGN KEY REFERENCES Reader(ReaderID),
	LoanData DATE NOT NULL DEFAULT GETDATE(),
	DueDate DATE NOT NULL,
	ReturnDate DATE NULL,
	FineAmount MONEY DEFAULT 0,
) ON FG_Indexes;

CREATE INDEX IX_Loan_Books
ON Loans (BooksID);

CREATE INDEX IX_Loans_Reader
ON Loans (ReaderID);

CREATE INDEX IX_Loans_Return
ON Loans(ReturnDate);


------------------------------------------------------------------------------------------------
CREATE TABLE FINES (
    FineID INT PRIMARY KEY,
    LoanID INT NOT NULL,
    ReaderID INT NOT NULL,
    Amount MONEY NOT NULL,
    CalculatedAt DATETIME NOT NULL,
    IsPaid BIT NOT NULL DEFAULT 0,
    FOREIGN KEY (LoanID) REFERENCES Loans(LoanID),
    FOREIGN KEY (ReaderID) REFERENCES Reader(ReaderID)
)ON FG_Indexes;


------------------------------------------------------------------------------------------------
CREATE TABLE LoansArchive (
	LoansArchive INT PRIMARY KEY IDENTITY(1, 1),
	LoanID INT NOT NULL FOREIGN KEY REFERENCES Loans(LoanID),
	BookID INT NOT NULL FOREIGN KEY REFERENCES Books(BookID),
	ReaderID INT NOT NULL FOREIGN KEY REFERENCES Reader(ReaderID),
	LeanData DATE NOT NULL,
	DueData DATE NOT NULL,
	ReturnData DATE NULL,
	FineAmount MONEY DEFAULT 0,
)ON FG_Archive;