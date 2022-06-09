#ifndef __TASKS_CURRENCY_UPDATE_HPP
#define __TASKS_CURRENCY_UPDATE_HPP

#include <lib/crypto/data_table.hpp>

namespace Tasks {
  /**
   * @brief Singleton class to control a single task designated to fetching and storing historical and current Cryptocurrency quotes.
   * @details Cryptocurrency quotes are fetched from using the coin_gecko API, and stored in the currencies 'database', defined in the
   * Crypto::Table data structure. The update period of a single currency is controlled by the update period.
   */
  class CurrencyUpdateTask {
   public:
    ~CurrencyUpdateTask() = default;
    /**
     * @brief Retrieve the singleton instance for the class.
     * @return CurrencyUpdateTask*
     */
    static CurrencyUpdateTask* instance();
    /**
     * @brief Start the currency update task. Quotes (prices) will be fetch and added to the currencies Crypto::Table data structure.
     * @details Currencies are fetched in the order defined Crypto::Table. The currency's data is only updated if the enabled field in
     * Crypto::Table is set to true.
     */
    void start();
    /**
     * @brief Stop the currency update task.
     * @details The task can be restarted with start(). However, there is no guarantee that the sequence is resumed from where it was
     * stopped.
     */
    void stop();
    /**
     * @brief Update the pricesDB database with historical data.
     * @details This function fetches the historical prices at an interval of 5 minutes. The number of historical points fetched is equal to
     * the number of entries available in the database. E.g, with 288 points = (288 / 12) = 24 hours. The update can take a long time,
     * around 15 seconds per currency.
     *
     * The currency is not updated if its enabled field in Cyrpto::Table is not set to true. In this case, the fuction returns 0
     * @param currency The target currency to update.
     * @warning All current entries located in the database will be overwritten.
     * @return size_t http status code for requests. 200 = success. 0 is returned if the currency's Crypto::Table enable field is set to
     * false.
     */
    size_t updateHistorical(Crypto::Currency currency);
    /**
     * @brief Query if the currency update task is running;
     * @return true Task is running.
     * @return false Task is not running.
     */
    bool running();
    /**
     * @brief Set the update period at which a single currency is updated. Defaults to 300 seconds.
     * @warning Be sure to check the the (update period / number of currencies) does not exceed the rate limit for coin_gecko
     * @param seconds The update period in seconds.
     */
    void updatePeriod(size_t seconds);
    /**
     * @brief Query the current update period being used.
     * @return size_t The period in seconds.
     */
    size_t updatePeriod();
    /**
     * @brief Structure used to describe a request which failed.
     */
    struct FailedRequest {
      bool valid;            ///< The failed request structure is valid. All other fields are invalid if this field is set to false.
      const char* currency;  ///< The crypto currency used for the request.
      const char* fiat;      ///< The Fiat currency used for the request.
      size_t unixTime;       ///< The Unix time stamp at which the request occurred.
      size_t returnCode;     ///< The return code. Usually this would be an HTTP status code. However, other values can exist.
    };
    /**
     * @brief Get information on the last failed request.
     * @details If a request has never failed, FailedRequest::valid will be set to false.
     * @return FailedRequest Description of failed request.
     */
    FailedRequest getLastFailedRequest() const;
    /**
     * @brief Get the total number of requests conducted.
     * @details This count includes initial historical, periodic and failed requests.
     * @return size_t Request count
     */
    size_t totalRequestCount() const;
    /**
     * @brief Get the total number of failed requests.
     * @return size_t Failed request count.
     */
    size_t failedRequestCount() const;
    /**
     * @brief Get the unix timestamp for the latest request.
     * @return size_t Latest request Unix timestamp
     */
    size_t lastRequestTimestamp() const;


   private:
    CurrencyUpdateTask();
    size_t _updatePeriod;
    bool _active;

    // Disable copy and move assignment operators and constructors.
    CurrencyUpdateTask(const CurrencyUpdateTask&) = default;
    CurrencyUpdateTask(CurrencyUpdateTask&&) = default;
    CurrencyUpdateTask& operator=(const CurrencyUpdateTask&) = default;
    CurrencyUpdateTask operator=(CurrencyUpdateTask&&) = delete;
  };
  constexpr auto CurrencyUpdate = CurrencyUpdateTask::instance;
};  // namespace Tasks

#endif  //  __TASKS_CURRENCY_UPDATE_HPP