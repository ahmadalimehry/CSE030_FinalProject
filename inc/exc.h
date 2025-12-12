#include <exception>
class GoToMainMenu : public std::exception {};
// This exception is to signal the main control loop to go back to main menu