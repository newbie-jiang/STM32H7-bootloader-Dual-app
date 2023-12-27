/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 */

#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "parser.h"
#include "interface_cmd.h"

struct finsh_shell g_shell;
static char *finsh_prompt_custom = "msh >";

const char *finsh_get_prompt(void)
{
    return finsh_prompt_custom;
}

/**
 * @ingroup finsh
 *
 * This function get the prompt mode of finsh shell.
 *
 * @return prompt the prompt mode, 0 disable prompt mode, other values enable prompt mode.
 */
rt_uint32_t finsh_get_prompt_mode(void)
{
    return g_shell.prompt_mode;
}

/**
 * @ingroup finsh
 *
 * This function set the prompt mode of finsh shell.
 *
 * The parameter 0 disable prompt mode, other values enable prompt mode.
 *
 * @param prompt the prompt mode
 */
void finsh_set_prompt_mode(rt_uint32_t prompt_mode)
{
    g_shell.prompt_mode = prompt_mode;
}

static int finsh_getchar(void)
{
    return my_getchar();
}

/**
 * @ingroup finsh
 *
 * This function set the echo mode of finsh shell.
 *
 * FINSH_OPTION_ECHO=0x01 is echo mode, other values are none-echo mode.
 *
 * @param echo the echo mode
 */
void finsh_set_echo(rt_uint32_t echo)
{
    g_shell.echo_mode = (rt_uint8_t)echo;
}

/**
 * @ingroup finsh
 *
 * This function gets the echo mode of finsh shell.
 *
 * @return the echo mode
 */
rt_uint32_t finsh_get_echo()
{
    return g_shell.echo_mode;
}




//void finsh_run_line(struct finsh_parser *parser, const char *line)
//{
//}

static rt_bool_t shell_handle_history(struct finsh_shell *shell)
{
#if 1
    int i;
    putstr("\r");

    for (i = 0; i <= 60; i++)
        putchar(' ');
    putstr("\r");

#else
    putstr("\033[2K\r");
#endif
    putstr(FINSH_PROMPT);
	putstr(g_shell.line);
    return RT_FALSE;
}

static void shell_push_history(struct finsh_shell *shell)
{
    if (g_shell.line_position != 0)
    {
        /* push history */
        if (g_shell.history_count >= FINSH_HISTORY_LINES)
        {
            /* if current cmd is same as last cmd, don't push */
            if (memcmp(&g_shell.cmd_history[FINSH_HISTORY_LINES - 1], g_shell.line, FINSH_CMD_SIZE))
            {
                /* move history */
                int index;
                for (index = 0; index < FINSH_HISTORY_LINES - 1; index ++)
                {
                    memcpy(&g_shell.cmd_history[index][0],
                           &g_shell.cmd_history[index + 1][0], FINSH_CMD_SIZE);
                }
                memset(&g_shell.cmd_history[index][0], 0, FINSH_CMD_SIZE);
                memcpy(&g_shell.cmd_history[index][0], g_shell.line, g_shell.line_position);

                /* it's the maximum history */
                g_shell.history_count = FINSH_HISTORY_LINES;
            }
        }
        else
        {
            /* if current cmd is same as last cmd, don't push */
            if (g_shell.history_count == 0 || memcmp(&g_shell.cmd_history[g_shell.history_count - 1], g_shell.line, FINSH_CMD_SIZE))
            {
                g_shell.current_history = g_shell.history_count;
                memset(&g_shell.cmd_history[g_shell.history_count][0], 0, FINSH_CMD_SIZE);
                memcpy(&g_shell.cmd_history[g_shell.history_count][0], g_shell.line, g_shell.line_position);

                /* increase count and set current history position */
                g_shell.history_count ++;
            }
        }
    }
    g_shell.current_history = g_shell.history_count;
}

void shell(void)
{
    int ch;

    /* normal is echo mode */
    g_shell.echo_mode = 1;

    putstr(FINSH_PROMPT);

    while (1)
    {
        ch = finsh_getchar();
        if (ch < 0)
        {
            continue;
        }

        /*
         * handle control key
         * up key  : 0x1b 0x5b 0x41
         * down key: 0x1b 0x5b 0x42
         * right key:0x1b 0x5b 0x43
         * left key: 0x1b 0x5b 0x44
         */
        if (ch == 0x1b)
        {
            g_shell.stat = WAIT_SPEC_KEY;
            continue;
        }
        else if (g_shell.stat == WAIT_SPEC_KEY)
        {
            if (ch == 0x5b)
            {
                g_shell.stat = WAIT_FUNC_KEY;
                continue;
            }

            g_shell.stat = WAIT_NORMAL;
        }
        else if (g_shell.stat == WAIT_FUNC_KEY)
        {
            g_shell.stat = WAIT_NORMAL;

            if (ch == 0x41) /* up key */
            {
                /* prev history */
                if (g_shell.current_history > 0)
                    g_shell.current_history --;
                else
                {
                    g_shell.current_history = 0;
                    continue;
                }

                /* copy the history command */
                memcpy(g_shell.line, &g_shell.cmd_history[g_shell.current_history][0],
                       FINSH_CMD_SIZE);
                g_shell.line_curpos = g_shell.line_position = strlen(g_shell.line);
                shell_handle_history(&g_shell);
                continue;
            }
            else if (ch == 0x42) /* down key */
            {
                /* next history */
                if (g_shell.current_history < g_shell.history_count - 1)
                    g_shell.current_history ++;
                else
                {
                    /* set to the end of history */
                    if (g_shell.history_count != 0)
                        g_shell.current_history = g_shell.history_count - 1;
                    else
                        continue;
                }

                memcpy(g_shell.line, &g_shell.cmd_history[g_shell.current_history][0],
                       FINSH_CMD_SIZE);
                g_shell.line_curpos = g_shell.line_position = strlen(g_shell.line);
                shell_handle_history(&g_shell);
                continue;
            }
            else if (ch == 0x44) /* left key */
            {
                if (g_shell.line_curpos)
                {
                    putstr("\b");
                    g_shell.line_curpos --;
                }

                continue;
            }
            else if (ch == 0x43) /* right key */
            {
                if (g_shell.line_curpos < g_shell.line_position)
                {
                    putchar(g_shell.line[g_shell.line_curpos]);
                    g_shell.line_curpos ++;
                }

                continue;
            }
        }

        /* received null or error */
        if (ch == '\0' || ch == 0xFF) continue;
        /* handle tab key */
        else if (ch == '\t')
        {
			/* 未实现 */
        }
        /* handle backspace key */
        else if (ch == 0x7f || ch == 0x08)
        {
            /* note that g_shell.line_curpos >= 0 */
            if (g_shell.line_curpos == 0)
                continue;

            g_shell.line_position--;
            g_shell.line_curpos--;

            if (g_shell.line_position > g_shell.line_curpos)
            {
                int i;

                memmove(&g_shell.line[g_shell.line_curpos],
                           &g_shell.line[g_shell.line_curpos + 1],
                           g_shell.line_position - g_shell.line_curpos);
                g_shell.line[g_shell.line_position] = 0;

                //putstr("\b%s  \b", &g_shell.line[g_shell.line_curpos]);
				putchar('\b');
				putstr(&g_shell.line[g_shell.line_curpos]);
				putstr("  \b");
				

                /* move the cursor to the origin position */
                for (i = g_shell.line_curpos; i <= g_shell.line_position; i++)
                    putstr("\b");
            }
            else
            {
                putstr("\b \b");
                g_shell.line[g_shell.line_position] = 0;
            }

            continue;
        }

        /* handle end of line, break */
        if (ch == '\r' || ch == '\n')
        {
            shell_push_history(&g_shell);
			if (g_shell.echo_mode)
				putstr("\r\n");

			/* 执行命令 */
            msh_exec(g_shell.line, g_shell.line_position);
            
            putstr(FINSH_PROMPT);
            memset(g_shell.line, 0, sizeof(g_shell.line));
            g_shell.line_curpos = g_shell.line_position = 0;
            continue;
        }

        /* it's a large line, discard it */
        if (g_shell.line_position >= FINSH_CMD_SIZE)
            g_shell.line_position = 0;

        /* normal character */
        if (g_shell.line_curpos < g_shell.line_position)
        {
            int i;

            memmove(&g_shell.line[g_shell.line_curpos + 1],
                       &g_shell.line[g_shell.line_curpos],
                       g_shell.line_position - g_shell.line_curpos);
            g_shell.line[g_shell.line_curpos] = ch;
            if (g_shell.echo_mode)
                putstr(&g_shell.line[g_shell.line_curpos]);

            /* move the cursor to new position */
            for (i = g_shell.line_curpos; i < g_shell.line_position; i++)
                putstr("\b");
        }
        else
        {
            g_shell.line[g_shell.line_position] = ch;
            if (g_shell.echo_mode)
                putchar(ch);
        }

        ch = 0;
        g_shell.line_position ++;
        g_shell.line_curpos++;
        if (g_shell.line_position >= FINSH_CMD_SIZE)
        {
            /* clear command line */
            g_shell.line_position = 0;
            g_shell.line_curpos = 0;
        }
    } /* end of device read */
}


