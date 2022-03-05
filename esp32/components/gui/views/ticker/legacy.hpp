#ifndef __GUI_VIEW_QUOTECHART_HPP
#define __GUI_VIEW_QUOTECHART_HPP

#include <lvgl.h>

#include "../../widgets/composite/quote_banner.hpp"
#include "../../widgets/elemental/chart.hpp"
#include "ticker.hpp"

namespace GUI {
  namespace Views {
    class Legacy : public Ticker {
     public:
      static Legacy* instance();
      void show() override;
      void hide() override;
      void setCurrentQuote(const float quote) override;
      void clearCurrentQuote() override;

      void setDailyDelta(const float percent) override;
      void clearDailyDelta() override;
      void setName(const char* text) override;
      void clearName() override;
      void setCurrencySymbol(const char symbol) override;
      void clearCurrencySymbol() override;
      void setIcon(const lv_img_dsc_t* src) override;
      void clearIcon() override;
      void plotValue(const float value) override;
      void clearPlot() override;
      void setPlotRange(const float min, const float max) override;
      void setPlotPointCount(size_t count) override;

      // Not used by this view.
      void setHourlyDelta(const float percent) override {}
      void clearHourlyDelta() override {}
      void setDailyMax(const float value) override {}
      void clearDailyMax() override {}
      void setDailyMin(const float value) override {}
      void clearDailyMin() override {}
      void setDailyChange(const float value) override {}
      void clearDailyChange() override {}
      void setWeeklyDelta(const float percent) override {}
      void clearWeeklyDelta() override {}
      void setTickerQuote(const char* text) override {}
      void clearTickerQuote() override {}
      void setTickerBase(const char* text) override {}
      void clearTickerBase() override {}

     private:
      Legacy();
      lv_obj_t* _screen;
      lv_obj_t* _empty;
      lv_obj_t* _container;
      Widgets::QuoteBanner _banner;
      Widgets::Chart _chart;
      lv_style_t _innerContainerStyle;
    };

  }  // namespace Views
  constexpr auto LegacyScreen = Views::Legacy::instance;
};  // namespace GUI


#endif  // __GUI_VIEW_QUOTECHART_HPP