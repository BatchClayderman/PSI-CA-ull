# PSI-CA

This is a possible simulation implementation of the PSI-CA cryptography scheme in C/C++ programming language, which is the baseline of the [OPSI-CA](https://github.com/BatchClayderman/OPSI-CA-ull), the [SPSI-CA](https://github.com/BatchClayderman/SPSI-CA-ull), and the [VPSI-CA](https://github.com/BatchClayderman/VPSI-CA-ull) cryptography schemes. 

The datum type used is ``unsigned long long int`` (64-bit). The network communication is simulated by memory copying. 

There are some differences between the official implementations and this repository due to the baseline comparison requirements. 

This repository is a part of [the cryptography schemes](https://github.com/BatchClayderman/Cryptography-Schemes). 

### Timing

For time consumption computation in or after September 2024, better time consumption computation can be done. 

The recent period has witnessed the ``#include<chrono>`` reach a computation level of nanoseconds. Users can modify the time consumption computation codes in this repository to make more exact timing. 

The following codes may be useful for cross-platform universal improvements. 

```
#if defined WIN32 || defined _WIN32 || defined _WIN64
#include <windows.h>
#ifndef TIME_POINT_TYPE
#define TIME_POINT_TYPE chrono::steady_clock::time_point
#endif
#else
#include <string.h>
#include <math.h>
#ifndef TIME_POINT_TYPE
#define TIME_POINT_TYPE chrono::system_clock::time_point
#endif
#endif
```

```
const TIME_POINT_TYPE startTime = chrono::high_resolution_clock::now();
const long long int timeDelta = (chrono::high_resolution_clock::now() - startTime).count();
cout << "Time: " << timeDelta << " ns" << endl;
```
