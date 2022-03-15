#ifndef __TASKS_CURRENCY_UPDATE_HPP
#define __TASKS_CURRENCY_UPDATE_HPP

#include <containers/crypto.hpp>

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
     * @details Currencies are fetched in the order defined Crypto::Table. The currency's data is always updated, even if the enabled field
     * is set to false.
     */
    void start();
    /**
     * @brief Stop the currency update task.
     * @details The task can be restarted with start(). However, there is no guarantee that the sequence is resumed from where it was
     * stopped.
     */
    void stop();
    /**
     * @brief Get the current currency being updated. Only relevent during historical update.
     * @details A currency is considered updated from the start of the request, during the request, and while waiting for the next
     * currencies turn to update.
     * @return Crypto::Currency
     */
    Crypto::Currency currentCurrency();
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


   private:
    CurrencyUpdateTask();
    size_t _updatePeriod;
    Crypto::Currency _currentCurrency;
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