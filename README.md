# QbS
This is the source code of the SIGMOD'21 paper "Query-by-Sketch: Scaling Shortest Path Graph Queries on Very Large Graphs".


PathPPL and ParentPPL are for 2-hop path cover.

preprocess.cpp is for all kinds of graphs. It cleans the data by removing duplicated edges, sorting vertices according to degrees and sorting edges according to degrees.

sparsify.cpp is for files generated by preprocess.cpp. It sparsifies the graph by removing edges adjacent to landmarks (top-k vertices).

index_construction.cpp construct labels for files generated by preprocess.cpp. It constructs labels using top-k vertices as landmarks.

queryQbS.cpp queries for shortest path graphs.
