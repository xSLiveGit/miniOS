#ifndef _OS_CONSOLER_INTERPRETER_H_
#define _OS_CONSOLER_INTERPRETER_H_

#define CSL_COMMAND_HELP "help"
#define CSL_COMMAND_DUMP_TRAPFRAME "dmptrapframe"
#define CSL_COMMAND_DUMP_TIMER "timer"
#define CSL_COMMAND_DIVISION_BY_ZERO "divbyzero"
#define CSL_COMMAND_HEAP_SCENARIO_1 "heap1"
#define CSL_COMMAND_HEAP_SCENARIO_2 "heap2"
#define CSL_COMMAND_HEAP_SCENARIO_3 "heap3"


void CslInterpretCmd(char* Command);

#endif //! _OS_CONSOLER_INTERPRETER_H_