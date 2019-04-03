## Own version of multithreaded std::accumulate

Napisz równoległą wersję algorytmu std::accumulate, który będzie współbieżnie sumował fragmenty kontenera. Wyniki powinny zostać również zapisane w kontenerze.
  - on empty returns init
  - calculate number of threads - `hardware_threads = hardware_concurrency() != 0 ? hardware_concurrency() : 2`
  - divide on blocks according to the number of threads
  - create vector of results
  - run every thread in parallel
  - put results in vector element passed by reference (threads cannot return a value)
  - join threads
  - accumulate results from the result vector
  - test on 1M elements and compare with standard std::accumulate
  - compare with std::par execution policy in std::accumulate from C++17 ;)
  - templatize algorithm on Iterator (depends on container) and type T (usually int, double)
