# Call Detail Record (CDR) Processing System

A system for processing and analyzing Call Detail Records with user authentication, customer billing, and interoperator settlement billing capabilities.

## Project Structure

```
CallDetailRecord/
├── Header/                     # Header files containing declarations and definitions
│   ├── mainHeader.h           # Core definitions, types, and includes
│   ├── menuHeader.h           # Menu-related function declarations
│   ├── utilityHeader.h        # Utility function declarations
│   ├── custHeader.h           # Customer billing related definitions
│   ├── interopHeader.h        # Interoperator billing related definitions
│   └── multithreadingHeader.h # Multithreading functionality declarations
│
├── src/                       # Source code files
│   ├── main.c                # Main program entry point and authentication flow
│   │
│   ├── Auth/                 # Authentication related implementations
│   │   ├── Login.c          # User login functionality
│   │   └── SignUp.c         # User signup functionality
│   │
│   ├── Billing/             # Billing related implementations
│   │   ├── process.c        # Main billing process with multithreading
│   │   ├── CustomerBilling.c    # Customer billing calculations
│   │   └── InteroperatorBilling.c # Interoperator settlement billing
│   │
│   ├── Menu/                # Menu handling implementations
│   │   └── menuImpl.c       # Menu display and handling functions
│   │
│   └── Utility/             # Utility functions
│       └── utility.c        # Common utility functions (file paths, etc.)
│
├── Data/                     # Data files
│   ├── data.cdr             # Input CDR data file
│   └── Users.txt            # User authentication data
│
└── Output/                  # Generated output files
    ├── CB.txt              # Customer Billing results
    └── ISOB.txt            # Interoperator Settlement Billing results
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

## Compilation

To compile the project, use:
```bash
gcc -I./Header src/main.c src/Menu/menuImpl.c src/Utility/utility.c src/Billing/process.c src/Billing/CustomerBilling.c src/Billing/InteroperatorBilling.c src/Auth/Login.c src/Auth/SignUp.c -o cdr_system.exe
```