#include "functions.hpp"

int getch()
{
	#if defined(__linux__) || defined(__APPLE__)
		struct termios oldt, newt;
		int ch;
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		ch = getchar();
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		return ch;
	#elif defined(_WIN32) || defined(_WIN64)
		return _getch();
	#endif
}

int kbhit(void)
{
	#if defined(__linux__) || defined(__APPLE__)
		struct termios oldt, newt;
		int ch;
		int oldf;

		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
		fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

		ch = getchar();

		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		fcntl(STDIN_FILENO, F_SETFL, oldf);

		if (ch != EOF)
		{
			ungetc(ch, stdin);
			return 1;
		}

		return 0;
	#elif defined(_WIN32) || defined(_WIN64)
		return _kbhit();
	#endif
}

/**
 * Map a value given from degree to a position for the motors
 * @param value Value in degree
 * @param startMin Min value at the beginning
 * @param startMax Min value at the beginning
 * @param stopMin Min value at the end
 * @param stopMax Min value at the end
 * @returns The value mapped
 */
unsigned mapping (unsigned value, const unsigned startMin, const unsigned startMax, const unsigned stopMin, const unsigned stopMax)
{
	value = value < startMin ? startMin : value;
	value = value > startMax ? startMax : value;

	return (value - startMin) * (stopMax - stopMin) / (startMax - startMin) + stopMin;
}