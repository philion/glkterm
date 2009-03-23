/* gtw_buf.h: The buffer window header
        for GlkTerm, curses.h implementation of the Glk API.
    Designed by Andrew Plotkin <erkyrath@netcom.com>
    http://www.eblong.com/zarf/glk/index.html
*/

/* Word types. */
#define wd_Text (1) /* Nonwhite characters */
#define wd_Blank (2) /* White (space) characters */
#define wd_EndLine (3) /* End of line character */
#define wd_EndPage (4) /* End of the whole text */

/* One word */
typedef struct tbword_struct {
    short type; /* A wd_* constant */
    short style;
    long pos; /* Position in the chars array. */
    long len; /* This is zero for wd_EndLine and wd_EndPage. */
} tbword_t;

/* One style run */
typedef struct tbrun_struct {
    short style;
    long pos;
} tbrun_t;

/* One laid-out line of words */
typedef struct tbline_struct {
    int numwords;
    tbword_t *words;
    
    long pos; /* Position in the chars array. */
    long len; /* Number of characters, including blanks */
    int startpara; /* Is this line the start of a new paragraph, or is it
        wrapped? */
    int printwords; /* Number of words to actually print. (Excludes the last
        blank word, if that goes outside the window.) */
} tbline_t;

typedef struct window_textbuffer_struct {
    window_t *owner;
    
    char *chars;
    long numchars;
    long charssize;
    
    int width, height;
    
    long dirtybeg, dirtyend; /* Range of text that has changed. */
    long dirtydelta; /* The amount the text has grown/shrunk since the
        last update. Also the amount the dirty region has grown/shrunk;
        so the old end of the dirty region == (dirtyend - dirtydelta). 
        If dirtybeg == -1, dirtydelta is invalid. */
    int drawall; /* Does the whole window need to be redrawn at the next
        update? (Set when the text is scrolled, for example.) */
    
    tbline_t *lines;
    long numlines;
    long linessize;
    
    tbrun_t *runs;
    long numruns;
    long runssize;

    /* Temporary lines; used during layout. */
    tbline_t *tmplines; 
    long tmplinessize;

    /* Temporary words; used during layout. */
    tbword_t *tmpwords;
    long tmpwordssize;

    /* Command history. */
    char **history;
    int historypos;
    int historyfirst, historypresent;

    long scrollline;
    long scrollpos;
    long lastseenline;
    
    /* for line input */
    char *inbuf;
    int inmax;
    long infence;
    long incurs;
    glui32 origstyle;
    gidispatch_rock_t inarrayrock;
} window_textbuffer_t;

extern chtype win_textbuffer_styleattrs[style_NUMSTYLES];

extern window_textbuffer_t *win_textbuffer_create(window_t *win);
extern void win_textbuffer_destroy(window_textbuffer_t *dwin);
extern void win_textbuffer_rearrange(window_t *win, grect_t *box);
extern void win_textbuffer_redraw(window_t *win);
extern void win_textbuffer_update(window_t *win);
extern void win_textbuffer_putchar(window_t *win, char ch);
extern void win_textbuffer_clear(window_t *win);
extern void win_textbuffer_trim_buffer(window_t *win);
extern void win_textbuffer_place_cursor(window_t *win, int *xpos, int *ypos);
extern void win_textbuffer_set_paging(window_t *win, int forcetoend);
extern void win_textbuffer_init_line(window_t *win, char *buf, int maxlen, int initlen);
extern void win_textbuffer_cancel_line(window_t *win, event_t *ev);

extern void gcmd_buffer_accept_key(window_t *win, glui32 arg);
extern void gcmd_buffer_accept_line(window_t *win, glui32 arg);
extern void gcmd_buffer_insert_key(window_t *win, glui32 arg);
extern void gcmd_buffer_move_cursor(window_t *win, glui32 arg);
extern void gcmd_buffer_delete(window_t *win, glui32 arg);
extern void gcmd_buffer_history(window_t *win, glui32 arg);
extern void gcmd_buffer_scroll(window_t *win, glui32 arg);

