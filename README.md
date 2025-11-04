# Call Detail Record (CDR) Processing System

A comprehensive system for processing and analyzing Call Detail Records (CDR) with secure user authentication, detailed customer billing, and interoperator settlement billing capabilities. The system efficiently processes telecom records and generates detailed billing reports for both individual customers and operator settlements using multi-threaded processing.

## Project Structure

```
CallDetailRecord/
├── Header/                     # Header files containing declarations and definitions
│   ├── custHeader.h           # Customer billing related definitions
│   ├── interopHeader.h        # Interoperator billing related definitions
│   ├── mainHeader.h           # Core definitions, types, and includes
│   ├── menuHeader.h           # Menu-related function declarations
│   ├── multithreadingHeader.h # Multithreading functionality declarations
│   └── utilityHeader.h        # Utility function declarations
│
├── src/                       # Source code files
│   ├── main.c                # Main program entry point
│   │
│   ├── Auth/                 # Authentication related implementations
│   │   ├── Login.c          # User login functionality
│   │   └── SignUp.c         # User signup functionality
│   │
│   ├── Billing/             # Billing related implementations
│   │   ├── CustomerBilling.c     # Customer billing search and display
│   │   └── InteroperatorBilling.c # Interoperator settlement search and display
│   │
│   ├── Menu/                # Menu handling implementations
│   │   └── menuImpl.c       # Menu display and handling functions
│   │
│   ├── Process/            # Data processing implementations
│   │   ├── process.c       # Main processing with multithreading
│   │   ├── CustomerBillingProcess.c    # Customer billing processing
│   │   └── InteroperatorBillingProcess.c # Interoperator billing processing
│   │
│   └── Utility/             # Utility functions
│       └── utility.c        # Common utility functions (file paths, etc.)
│
├── Data/                     # Data files
│   ├── data.cdr             # Input CDR data file
│   └── Users.txt            # User authentication data
│
├── Output/                  # Generated output files
│   ├── CB.txt              # Customer Billing results
│   └── ISOB.txt           # Interoperator Settlement Billing results
│
└── logs/                   # System logs directory
```

## Component Descriptions

### Header Files

- **mainHeader.h**: Contains core system definitions, types, and includes. Defines constants, enums for menu choices, and authentication status codes.
- **menuHeader.h**: Declares all menu-related functions for displaying and handling user interface menus.
- **utilityHeader.h**: Declares utility functions for file path handling and common operations.
- **custHeader.h**: Defines structures and functions for customer billing processing.
- **interopHeader.h**: Defines structures and functions for interoperator settlement billing.
- **multithreadingHeader.h**: Contains declarations for parallel processing of billing data.

### Source Files

#### Main Program
- **main.c**: Entry point of the program. Implements the main authentication loop and program flow control.

#### Authentication Module
- **Login.c**: Implements user login verification and authentication.
- **SignUp.c**: Handles new user registration with email and password validation.

#### Billing Module
- **process.c**: Implements multithreaded processing of CDR data.
- **CustomerBilling.c**: Processes and calculates customer-specific billing data.
- **InteroperatorBilling.c**: Handles interoperator settlement calculations.

#### Menu Module
- **menuImpl.c**: Implements all menu display and handling functions including:
  - Authentication menu
  - CDR processing menu
  - Billing search menu
  - Customer and Interoperator billing menus

#### Utility Module
- **utility.c**: Implements common utility functions for:
  - File path management
  - Directory creation
  - Input/Output operations

### Data Directories

- **Data/**: Contains input data files
  - `data.cdr`: Raw Call Detail Record data
  - `Users.txt`: Encrypted user credentials

- **Output/**: Contains generated billing reports
  - `CB.txt`: Customer Billing report
  - `ISOB.txt`: Interoperator Settlement Billing report

## Features

### 1. Authentication System
- Secure signup and login functionality
- Password encryption
- Session management
- Input validation

### 2. CDR Processing
- Multi-threaded processing of CDR records
- Supports multiple record types:
  - Voice calls (MOC/MTC)
  - SMS messages (SMS-MO/SMS-MT)
  - GPRS (data) usage
- Efficient data structures (Hash tables) for O(1) lookup
- Concurrent processing for improved performance

### 3. Customer Billing (CB.txt)
- Individual customer usage tracking:
  - Voice calls (incoming/outgoing)
  - SMS messages (incoming/outgoing)
  - Data usage (upload/download)
- Service categorization:
  - Within same operator
  - Cross-operator services
- Search functionality by MSISDN
- Detailed billing reports

### 4. Interoperator Settlement (ISOB.txt)
- Operator-level settlement processing
- Traffic analysis:
  - Voice call durations
  - SMS message counts
  - Data service usage
- Search by operator name or MMC/MNC code
- Comprehensive settlement reports

## Building and Running

### Prerequisites
- GCC Compiler
- Windows OS (for current implementation)
- Minimum 512MB RAM
- 1GB free disk space

### Compilation
```bash
gcc -I./Header src/main.c src/Menu/menuImpl.c src/Utility/utility.c src/Process/process.c src/Process/CustomerBillingProcess.c src/Process/InteroperatorBillingProcess.c src/Auth/Login.c src/Auth/SignUp.c src/Billing/InteroperatorBilling.c src/Billing/CustomerBilling.c -o main.exe
```

### Running the Application
```bash
./main.exe
```

### Usage Flow
1. Launch the application
2. Sign up (new users) or Log in (existing users)
3. Select from main menu:
   - Process CDR data
   - Search/View billing information
4. Choose billing type:
   - Customer Billing
     - Search by MSISDN
     - View complete report
   - Interoperator Settlement
     - Search by operator
     - View full settlement