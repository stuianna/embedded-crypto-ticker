#ifndef __GUI_VIEWS_TICKER_HPP
#define __GUI_VIEWS_TICKER_HPP

#include <lvgl.h>

#include <cstdint>

#include "../view.hpp"

namespace GUI {
  namespace Views {
    class Ticker : public View {
      virtual void setCurrentQuote(const float quote) = 0;
      virtual void clearCurrentQuote() = 0;
      virtual void setHourlyDelta(const float percent) = 0;
      virtual void clearHourlyDelta() = 0;
      virtual void setDailyDelta(const float percent) = 0;
      virtual void clearDailyDelta() = 0;
      virtual void setDailyMax(const float value) = 0;
      virtual void clearDailyMax() = 0;
      virtual void setDailyMin(const float value) = 0;
      virtual void clearDailyMin() = 0;
      virtual void setDailyChange(const float value) = 0;
      virtual void clearDailyChange() = 0;
      virtual void setWeeklyDelta(const float percent) = 0;
      virtual void clearWeeklyDelta() = 0;
      virtual void setName(const char* text) = 0;
      virtual void clearName() = 0;
      virtual void setTickerQuote(const char* text) = 0;
      virtual void clearTickerQuote() = 0;
      virtual void setTickerBase(const char* text) = 0;
      virtual void clearTickerBase() = 0;
      virtual void setCurrencySymbol(const char symbol) = 0;
      virtual void clearCurrencySymbol() = 0;
      virtual void setIcon(const lv_img_dsc_t* src) = 0;
      virtual void clearIcon() = 0;
      virtual void plotValue(const float value) = 0;
      virtual void clearPlot() = 0;
      virtual void setPlotRange(const float min, const float max) = 0;
      virtual void setPlotPointCount(size_t count) = 0;
    };
  }  // namespace Views
}  // namespace GUI

#endif  // __GUI_VIEWS_TICKER_HPP