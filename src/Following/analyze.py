import pstats
p = pstats.Stats('output.prof')
p.strip_dirs().sort_stats("cumtime").print_stats()