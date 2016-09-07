#include <pebble.h>
#include <FrenchCalendarConverter.h>
#include <time.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_day_layer;
static TextLayer *s_date_layer;
static TextLayer *s_year_layer;

static void update_time() {

	// Get a tm structure
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	int daysFromBeginningToEpoch = 64748;
	int absolute = daysFromBeginningToEpoch + (int) time(NULL)/(60*60*24);

	APP_LOG(APP_LOG_LEVEL_DEBUG, "Total absolute days: %d", absolute);//tick_time->tm_year);

	//go from number of days to french date
	int year = 1;
	//TODO: I can probably just rework this to start at epoch date
	//Full Years
	while (absolute >= DaysInFrenchYear(year)) {
		absolute -= DaysInFrenchYear(year);
		year++;
	}

	APP_LOG(APP_LOG_LEVEL_DEBUG, "absolute days after removing full years : %d", absolute);

	//Full Months
	int month = 0; //start with vendémiaire
	while (absolute >= DaysInFrenchMonth(year, month)) {
		absolute -= DaysInFrenchMonth(year, month);
		month++;
	}
	const char* monthName = GetMonthName(month);

	APP_LOG(APP_LOG_LEVEL_DEBUG, "absolute days after removing full months (and the day of the french month): %d", absolute);


	int day = absolute;

	const char* dayName = GetDayName(day % 10);
	//adding one since day gets initialized to 0
	day++;

	APP_LOG(APP_LOG_LEVEL_DEBUG, "The date is %s the %d of %s %d ", dayName, day, monthName, year);  

	APP_LOG(APP_LOG_LEVEL_DEBUG, "Loop index now %d", tick_time->tm_yday);

	// Write the current hours and minutes into a buffer
	static char s_buffer[8];
	strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
	"%H:%M" : "%I:%M", tick_time);

	// Display this time on the TextLayer
	text_layer_set_text(s_time_layer, s_buffer);

	//display the day
	char *dayLine = (char*)malloc(15 * sizeof(char));
	snprintf(dayLine, 15,"%s", dayName);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Loop index now %s", dayLine);
	text_layer_set_text(s_day_layer, dayLine);

	//display the date
	char *dateLine = (char*)malloc(15 * sizeof(char));
	snprintf(dateLine, 15,"%d %s", day, monthName);
	text_layer_set_text(s_date_layer, dateLine);

	//display the year
	text_layer_set_text(s_year_layer, GetRomanizedYear(year));
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  //clock_t start = clock(), diff;
  int timeBefore = (int) time(NULL);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Begin");
  update_time();
  int timeAfter = (int) time(NULL);
  int execTime = (timeAfter - timeBefore);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "END: %d", execTime);
  //diff = clock() - start;

  //int msec = diff * 1000 / CLOCKS_PER_SEC;
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Time taken %d seconds %d milliseconds", msec/1000, msec%1000);
	
}

static void main_window_load(Window *window) {
	// Get information about the Window
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	// Create the time TextLayer with specific bounds
	s_time_layer = text_layer_create(
	GRect(0, PBL_IF_ROUND_ELSE(30, 26), bounds.size.w, 30));
	APP_LOG(APP_LOG_LEVEL_DEBUG, "dimensions: %d x %d", bounds.size.w,bounds.size.h);

	// Improve the layout to be more like a watchface
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorBlack);
	text_layer_set_text(s_time_layer, "00:00");
	text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

	// Add it as a child layer to the Window's root layer
	layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

	
	// Create the day TextLayer with specific bounds
	s_day_layer = text_layer_create(
	GRect(0, PBL_IF_ROUND_ELSE(70, 66), bounds.size.w, 30));

	text_layer_set_background_color(s_day_layer, GColorClear);
	text_layer_set_text_color(s_day_layer, GColorBlack);
	text_layer_set_text(s_day_layer, "day");
	text_layer_set_font(s_day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);

	layer_add_child(window_layer, text_layer_get_layer(s_day_layer));

	// Create the month TextLayer with specific bounds
	s_date_layer = text_layer_create(
	GRect(0, PBL_IF_ROUND_ELSE(90, 86), bounds.size.w, 30));

	text_layer_set_background_color(s_date_layer, GColorClear);
	text_layer_set_text_color(s_date_layer, GColorBlack);
	text_layer_set_text(s_date_layer, "month");
	text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

	layer_add_child(window_layer, text_layer_get_layer(s_date_layer));

	// Create the year TextLayer with specific bounds
	s_year_layer = text_layer_create(
	GRect(0, PBL_IF_ROUND_ELSE(110, 106), bounds.size.w, 30));

	text_layer_set_background_color(s_year_layer, GColorClear);
	text_layer_set_text_color(s_year_layer, GColorBlack);
	text_layer_set_text(s_year_layer, "year");
	text_layer_set_font(s_year_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(s_year_layer, GTextAlignmentCenter);

	layer_add_child(window_layer, text_layer_get_layer(s_year_layer));
}

static void main_window_unload(Window *window) {
	// Destroy TextLayer
	text_layer_destroy(s_time_layer);
	text_layer_destroy(s_day_layer);
	text_layer_destroy(s_date_layer);
}

static void init() {
	// Create main Window element and assign to pointer
	s_main_window = window_create();

	// Set handlers to manage the elements inside the Window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});

	// Show the Window on the watch, with animated=true
	window_stack_push(s_main_window, true);

	// Register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

	// Make sure the time is displayed from the start
	update_time();
}

static void deinit() {
	// Destroy Window
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}