#ifdef GLCONSOLE_SUPPORT

void ConsoleToggle();
bool ConsoleIsOpen();
void ConsoleRender();
void ConsoleInput (int key, int char_code);
void ConsoleUpdate ();
void ConsoleLog (const char* message, ...);

#endif