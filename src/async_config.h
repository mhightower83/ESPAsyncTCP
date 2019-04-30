#ifndef LIBRARIES_ESPASYNCTCP_SRC_ASYNC_CONFIG_H_
#define LIBRARIES_ESPASYNCTCP_SRC_ASYNC_CONFIG_H_

#ifndef ASYNC_TCP_SSL_ENABLED
#define ASYNC_TCP_SSL_ENABLED 0
#endif

#define DEBUG_ESP_ASYNC_TCP 1
// #define DEBUG_ESP_TCP_SSL 1
// #define DEBUG_ESP_PORT Serial

#include <DebugPrintMacros.h>

#if defined(DEBUG_ESP_PORT) && !defined(TIME_STAMP_ASYNC_TCP_FMT)
#define TIME_STAMP_ASYNC_TCP_FMT "%06u.%03u "
struct _ASYNC_TCP_TIME_STAMP {
  unsigned dec;
  unsigned whole;
};
inline struct _ASYNC_TCP_TIME_STAMP asyncTcpTimeStamp(void) {
  struct _ASYNC_TCP_TIME_STAMP st;
  unsigned now = millis() % 1000000000;
  st.dec = now % 1000;
  st.whole = now / 1000;
  return st;
}
#endif

#if defined(DEBUG_ESP_PORT) && !defined(ASYNC_TCP_DEBUG_GENERIC)
  #define ASYNC_TCP_DEBUG_GENERIC( module, format, ... ) \
    do { \
      struct _ASYNC_TCP_TIME_STAMP st = asyncTcpTimeStamp(); \
      DEBUG_ESP_PORT.printf( TIME_STAMP_ASYNC_TCP_FMT module " " format, st.whole, st.dec, ##__VA_ARGS__ ); \
    } while(false)
#endif

#if defined(ASYNC_TCP_DEBUG_GENERIC) && !defined(ASYNC_TCP_ASSERT)
#define ASYNC_TCP_ASSERT_GENERIC( a, module ) \
  if ( !(a) ) \
    do { \
      ASYNC_TCP_DEBUG_GENERIC( module, "%s:%s:%u: ASSERT("#a") failed!\n", __FILE__, __func__, __LINE__); \
      DEBUG_ESP_PORT.flush(); \
    } while(false)
#endif

#ifndef ASYNC_TCP_DEBUG_GENERIC
#define ASYNC_TCP_DEBUG_GENERIC(...) do { (void)0;} while(false)
#endif

#ifndef ASYNC_TCP_ASSERT_GENERIC
#define ASYNC_TCP_ASSERT_GENERIC(...) do { (void)0;} while(false)
#endif

#if defined(DEBUG_ESP_ASYNC_TCP) && !defined(ASYNC_TCP_DEBUG)
#define ASYNC_TCP_DEBUG( format, ...) ASYNC_TCP_DEBUG_GENERIC("[ASYNC_TCP]", format, ##__VA_ARGS__)
#endif


#ifndef ASYNC_TCP_ASSERT
#define ASYNC_TCP_ASSERT( a ) ASYNC_TCP_ASSERT_GENERIC( (a), "[ASYNC_TCP]")
#endif

#if defined(DEBUG_ESP_TCP_SSL) && !defined(TCP_SSL_DEBUG)
// #define TCP_SSL_DEBUG(...) ets_printf(__VA_ARGS__)
#define TCP_SSL_DEBUG( format, ...) ASYNC_TCP_DEBUG_GENERIC("[TCP_SSL]", format, ##__VA_ARGS__)
#endif


#ifndef ASYNC_TCP_ASSERT
#define ASYNC_TCP_ASSERT(...) do { (void)0;} while(false)
#endif
#ifndef ASYNC_TCP_DEBUG
#define ASYNC_TCP_DEBUG(...) do { (void)0;} while(false)
#endif
#ifndef TCP_SSL_DEBUG
#define TCP_SSL_DEBUG(...) do { (void)0;} while(false)
#endif

#endif /* LIBRARIES_ESPASYNCTCP_SRC_ASYNC_CONFIG_H_ */
