#ifndef LIBRARIES_ESPASYNCTCP_SRC_ASYNC_CONFIG_H_
#define LIBRARIES_ESPASYNCTCP_SRC_ASYNC_CONFIG_H_

#ifndef ASYNC_TCP_SSL_ENABLED
#define ASYNC_TCP_SSL_ENABLED 0
#endif

#define DEBUG_ESP_ASYNC_TCP
#if defined(DEBUG_ESP_PORT) && defined(DEBUG_ESP_ASYNC_TCP) && !defined(ASYNC_TCP_DEBUG)
// Too much noise. #pragma message("ASYNC_TCP_DEBUG defined in async_config.h")
struct _TIME_STAMPER {
  unsigned dec;
  unsigned whole;
};
#define TIME_STAMPER_ASYNC_TCP_FMT "%06u.%03u [ASYNC_TCP] "
inline struct _TIME_STAMPER timeStamper(void) {
  struct _TIME_STAMPER st;
  unsigned now = millis() % 1000000000;
  st.dec = now % 1000;
  st.whole = now / 1000;
  return st;
}
#define ASYNC_TCP_DEBUG(format, ...) do { \
  struct _TIME_STAMPER st = timeStamper(); \
    DEBUG_ESP_PORT.printf( TIME_STAMPER_ASYNC_TCP_FMT format, st.whole, st.dec, ##__VA_ARGS__ ); \
    } while(false)
#else
#define ASYNC_TCP_DEBUG(format, ...) do {(void)0;} while(false)
#endif

#ifndef ASYNC_TCP_DEBUG
#define ASYNC_TCP_DEBUG(...) //ets_printf(__VA_ARGS__)
#endif
#ifndef TCP_SSL_DEBUG
#define TCP_SSL_DEBUG(...) //ets_printf(__VA_ARGS__)
#endif

#endif /* LIBRARIES_ESPASYNCTCP_SRC_ASYNC_CONFIG_H_ */
