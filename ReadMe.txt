1. I created packages:
-- DepAnalysis:
   Dependency analysis, providing the callable object for parallel dependency analysis.
   Inputs are working dir and type table. 
   Output is a map of dependency information.In the form of :
   filename:
   depend on: <list of files>.
   In this analysis global functions are handled.
-- ParaDepAnalysis:
   Core part of the program. Grab callable object from dependency analysis and type analysis, get tasks to handled them, with 4 threads.
-- ThreadPool:
   This is based on Prof. Fawcett's work, WorkItemBlockingQ. Turned it into a multi thread version.
-- TypeDefAnalysis:
   This package deals with type definitions. It generates two tables, one with only type definitions, including enum, class, aliase and strcut. The other also includes namespaces and function, for denpendency analysis.
-- Parser:
   Made modifications to the Parser from Proj2. Add rules and actions to handle type definitions and using of namespaces.
   
2. Please run.bat 
   If it doesn't work right, please run with visual studio, thank you!