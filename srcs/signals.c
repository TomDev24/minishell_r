#include "minishell.h"

void	set_param_tty()
{
	tcgetattr(0, &mshell.setting_out_tty);
	tcgetattr(0, &mshell.setting_tty);
	mshell.setting_tty.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &mshell.setting_tty);
}

void	unset_param_tty()
{
	tcsetattr(0, TCSANOW, &mshell.setting_out_tty);
}
/*
void    sigint_handler(int num)
{
    (void) num;
    waitpid(-1, NULL, 0);
//    write(1, "\nchild proc over!", 17);
//    write(1, "\n>>", 3);
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}
*/
void    sigint_handler(int num)
{
	int	retval;

	(void) num;
	retval = waitpid(-1, NULL, 0);
//	printf("retval = %d\n", retval);
//	write(1, "\nchild proc over!", 17);
	if (retval == -1 )
	{
//		rl_on_new_line();
//		rl_redisplay();
//		write(1, "  \n", 3);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
	{
//		printf("retval = %d\n", retval);
		write(1, "\n", 1);
	}
}

void	sigquit_handler(int num)
{
	(void) num;
}
