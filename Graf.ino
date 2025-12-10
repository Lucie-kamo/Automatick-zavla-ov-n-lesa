#include <Wire.h>
#include <U8glib.h>

// Konstruktor pro SSD1306 128x64 I2C
U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);

const int MAX_MONTHS = 3;

// pevné testovací hodnoty (říj=3, lis=7, pro=15)
int waterings[MAX_MONTHS] = {3, 7, 15};

// index aktuálního měsíce
int monthNumber = 11;
int currentMonthIndex = 2;

const char* monthNames[12] = {"led","uno","bre","dub","kve","cvn",
                              "cvc","srp","zar","rij","lis","pro"};

void setup() {
  // U8glib nevyžaduje begin()
}

void loop() {
  drawChart();
  delay(3000);
}

void drawChart() {
  oled.firstPage();
  do {
    drawChartPage();
  } while (oled.nextPage());
}

void drawChartPage() {

  const int chartTop = 10;        // horní okraj grafu
  const int chartBottom = 45;     // spodní okraj grafu posunutý nahoru, dole místo pro popisky
  const int chartHeight = chartBottom - chartTop;
  const int barWidth = 30;
  const int barSpacing = 5;

  oled.setFont(u8g_font_6x10);

  // Osa Y
  for (int yVal = 0; yVal <= 15; yVal += 5) {
    int y = chartBottom - (yVal * chartHeight / 15);
    oled.drawLine(10, y, 110, y);    // vodorovná čára
    oled.setPrintPos(0, y + 3);      // číslo
    oled.print(yVal);
  }

  // Sloupce
  for (int i = 0; i < MAX_MONTHS; i++) {
    int val = waterings[i];
    int x = 20 + i * (barWidth + barSpacing);
    int barH = val * chartHeight / 15;

    oled.drawFrame(x, chartBottom - barH, barWidth, barH);
    oled.drawBox(x + 1, chartBottom - barH + 1, barWidth - 2, barH - 2);

    // popisek měsíce pod sloupcem
    int m = (monthNumber - (MAX_MONTHS - 1 - i) + 12) % 12;
    oled.setPrintPos(x + 5, chartBottom + 10);
    oled.print(monthNames[m]);
  }
}
