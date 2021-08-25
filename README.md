# Financial-Computing-Final-C-
FRE-GY 6883 Financial Computing Course Team Projects

Project Tasks:

Task 1: Earnings research: sort stocks from Russell 1000 into 3 groups based on their earnings and EPS Estimate based Zacks.

Task 2: Project Design:

a) Create classes and data structure such as vectors, matrix, and maps.

b) Figure out how to handle historical price retrieval from Yahoo Finance for all IWB 1000 stocks and parse the retrieved data?

c) Figure out how to implement your Bootstrap algorithm?

d) Write member function or independent functions (with operator overloading) for all the calculation.

e) Design and implement menu

Task 3: Divide the project into modules and assign team members working on each module.

Task 4: Module Integration and Testing


Programming Requirements:
• Use liburl to retrieve historical price data from Yahoo Finance: A function retrieves the adjusted close prices for selected Russell 1000 stocks and IWB into memory.
• Create a set of classes such as class for stock to handle EPS estimate and price information.
• Use member functions or independent functions for all calculation. Overload a few arithmetic operators for vector/matrix.
• The stocks and their corresponding price information for each group should be stored in a STL map, with stock symbol as its keys.
• The expected AAR, AAR STD, and expected CAAR and CAAR STD for 3 groups are presented in a matrix. The row of the matrix is the group#, matrix columns are for AAR, AAR-STD, CAAR, CAAR-STD
• Use Excel Driver or gnuplot to show the CAAR from all 3 groups in one graph.
• Your program should be able to:
2
o Retrieve historical price data for all selected stocks. Parse the retrieved data for dates and adjusted closing prices.
o Calculate AAR, AAR-STD, CAAR CAAR-STD for each group
o Populate the stock maps and AAR/CAAR matrix.
o Show an Excel or gnuplot graph with CAAR for all 3 groups.
• Your program should have a menu of 5 options:
o Enter N to retrieve 2N+1 days of historical price data for all stocks (you need to validate user input to make sure N >= 30).
o Pull information for one stock from one group:
▪ Daily Prices
▪ Cumulative Daily Returns
▪ The group the stock belongs to
▪ Earning Announcement Date, Period Ending, Estimated, Reported Earnings, Surprise and Surprise %.
o Show AAR, AAR-SD, CAAR and CAAR-STD for one group.
o Show the Excel or gnuplot graph with CAAR for all 3 groups.
o Exit your program.
