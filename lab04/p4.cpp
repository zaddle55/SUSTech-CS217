#include <iostream>
using namespace std;
enum Day { MONDAY = 1, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY };
enum Weather { SUNNY, RAINY, CLOUDY, SNOWNY };

int main(/*complete code here if needed*/) {
  int d = 0;
  int w = 0;
  cout << "input the Day value: Monday(1), Tuesday(2), Wednesday(3), "
          "Thursday(4), Friday(5), Saturday(6), Sunday(7)\n ";
  cin >> d;

  cout << "This is: ";
  switch (d) {
  case MONDAY:
    cout << "Monday\n";
    break;
  case TUESDAY:
    cout << "Tuesday\n";
    break;
  case WEDNESDAY:
    cout << "Wednesday\n";
    break;
  case THURSDAY:
    cout << "Thursday\n";
    break;
  case FRIDAY:
    cout << "Friday\n";
    break;
  case SATURDAY:
    cout << "Saturday\n";
    break;
  case SUNDAY:
    cout << "Sunday\n";
    break;
  default:
    cout << "unknown\n";
    break;
  }

  cout << "input the Weather value: SUNNY(0), RAINY(1), CLOUDY(2), SNOWNY(3)\n";
  cin >> w;

  cout << "The weather is: ";

  switch (w) {
  case SUNNY:
    cout << "SUNNY\n";
    break;
  case RAINY:
    cout << "RAINY\n";
    break;
  case CLOUDY:
    cout << "ClOUDY\n";
    break;
  case SNOWNY:
    cout << "SNOWNY\n";
    break;
  default:
    cout << "unknown\n";
    break;
  }

  if (w == 0 && d > 5 && d < 8)
    cout << "can Travel\n";
  else
    cout << "not suitable for travelling\n";
  return 0;
}
