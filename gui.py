import os
import math

from gi.repository import Gtk, Gdk, GObject, GLib
from gi.repository.GdkPixbuf import Pixbuf, InterpType

# class RezChar(Gtk.Image):
#     def __init__(self, char, row, col, w, h):
#         """
#         row\col 0 1 2 3
#            0    x x x x
#            1    x x x x
#            2    x x x x
#            3    x x x x
#         """
#         Gtk.Image.__init__(self)
#         self.row = row
#         self.col = col
#         self.char = char
#         self._active = None
#         self._pre = None
#         self._post = None
#         self._width = w
#         self._height = h

#     def set_active(self, active, pre=None, post=None):
#         if self._active == active and self._pre == pre and self._post == post:
#             return
#         if active:
#             fname = "images/cell_%s_active.svg" %self.char
#             self._pre = pre
#             self._post = post
#         else:
#             fname = "images/cell_%s.svg" %self.char
#             self._pre = self._post = None

#         pre_arrow = self.get_super_fnames(pre)
#         post_arrow = self.get_super_fnames(post)
#         try:
#             pix = Pixbuf.new_from_file_at_size(fname, self._width, self._height)
#             if pre_arrow:
#                 pix_pre = Pixbuf.new_from_file_at_size(pre_arrow,
#                                                        self._width, self._height)
#                 pix_pre.composite(pix, 0, 0, pix.props.width, pix.props.height,
#                                   0, 0, 1.0, 1.0, InterpType.HYPER, 255)
#             if post_arrow:
#                 pix_post = Pixbuf.new_from_file_at_size(post_arrow,
#                                                         self._width, self._height)
#                 pix_post.composite(pix, 0, 0, pix.props.width, pix.props.height,
#                                    0, 0, 1.0, 1.0, InterpType.HYPER, 255)
                
#             self.set_from_pixbuf(pix)
#             self._active = active
#         except GLib.GError, e:
#             print 'error', e

#     def get_super_fnames(self, char):
#         """
#         Coordinate per la freccia:

#         (-1, -1) (-1, 0) (-1, 1)

#         (0, -1)           (0, 1)

#         (1, -1)   (1, 0)  (1, 1)

#         """
#         if char:
#             arrow_x, arrow_y = char.row - self.row, char.col -  self.col
#             if (arrow_x, arrow_y) != (0, 0):
#                 fname = "images/arrow_%d_%d.svg" %(arrow_x, arrow_y)
#                 if os.path.exists(fname):
#                     return fname
#         return None

#     def __str__(self):
#         return "%s at (%d, %d)" %(self.char, self.row, self.col)

# class RezTable(Gtk.EventBox):
#     _border = 10
#     def __init__(self):
#         Gtk.EventBox.__init__(self)
#         self.set_above_child(True)
#         self._sigtable = []

#     def popola(self, w, h, wm):
#         charmatrix = wm.schema
#         w = w / len(charmatrix)
#         h = h / len(charmatrix[0])
#         charw = w - self._border
#         charh = h - self._border
#         if charw < 1 or charh < 1:
#             charw = charh = 75

#         self._tab = Gtk.Table(wm.rows, wm.cols, True)
#         self._tab.set_row_spacings(0)
#         self._tab.set_col_spacings(0)
#         for i in range(wm.rows):
#             for j in range(wm.cols):
#                 b = RezChar(charmatrix[i][j], i, j, charw, charh)
#                 b.set_active(False)
#                 self._tab.attach(b, j, j+1, i, i+1,
#                                  Gtk.AttachOptions.EXPAND,
#                                  Gtk.AttachOptions.EXPAND)
#                 #b.set_property("expand", True)
#                 #self._tab.attach(b, j, i, 1, 1)

#         sigid = self.connect("button-press-event", self.button_press_cb)
#         self._sigtable.append(sigid)
#         sigid = self.connect("button-release-event", self.button_release_cb)
#         self._sigtable.append(sigid)
#         sigid = self.connect("motion-notify-event", self.motion_notify_cb)
#         self._sigtable.append(sigid)

#         self.add(self._tab)
#         self.wm = wm

#     def spopola(self):
#         for sigid in self._sigtable:
#             self.disconnect(sigid)
#         self._sigtable = []
#         self.remove(self._tab)

#     def _get_char_by_cursor(self, ev):
#         rect = self._tab.get_allocation()
#         if ev.x < 0 or ev.x > rect.width or ev.y < 0 or ev.y > rect.height:
#             #print "out of range"
#             return None

#         char_w = rect.width / self.wm.rows
#         char_h = rect.height / self.wm.cols

#         # considera il contorno attorno alle lettere
#         rel_x = ev.x % char_w
#         rel_y = ev.y % char_h
#         if rel_x < self._border or rel_x > (char_w - self._border) or\
#                 rel_y < self._border or rel_y > (char_h - self._border):
#             return None
#         col = int(ev.x / char_w)
#         row = int(ev.y / char_h)
#         for child in self._tab.get_children():
#             if child.row == row and child.col == col:
#                 return child
#         else:
#             print 'char inesistente pos', tab_x, tab_y
#             return None

#     def handle_char_pressed(self, char):
#         path = self.wm.put_char(char)
#         for c in self._tab.get_children():
#             if c not in path:
#                 c.set_active(False)
#         pre = cur = post = path[0]
#         i = 0
#         plen = len(path)
#         while i < plen:
#             if i < plen-1:
#                 post = path[i+1]
#             cur.set_active(True, pre, post)
#             pre, cur = cur, post
#             i += 1

#     def motion_notify_cb(self, obj, event):
#         # sembra che l'evento sia generato solo quando si clicca
#         # in caso contrario dovrei installare la callback in caso di button_press
#         char = self._get_char_by_cursor(event)
#         if char:
#             # continuo a raccogliere la sequenza di char
#             self.handle_char_pressed(char)

#     def button_press_cb(self, tab, event):
#         # inizio a raccogliere la sequenza di char
#         char = self._get_char_by_cursor(event)
#         if char:
#             self.handle_char_pressed(char)

#     def button_release_cb(self, tab, event):
#         # fine raccolta sequenza char
#         for child in self._tab.get_children():
#             child.set_active(False)

#         wchars = self.wm.stop_word()
#         word = ''.join([c.char for c in wchars])
#         if self.wm.check_word(word):
#             print "HEHEHEHE"


class RezCharNew(object):
    def __init__(self, row, col, char, value, opts):
        self.row, self.col, self.char = row, col, char
        self.value, self.options = value, opts
        self.active = False
        self.arrow_x = 0
        self.arrow_y = 0

    def __eq__(self, o):
        return self.row == o.row and self.col == o.col\
            and self.char == o.char and self.active == o.active\
            and self.arrow_x == o.arrow_x and self.arrow_y == o.arrow_y

class RezTableNew(Gtk.EventBox):
    _border = 10
    def __init__(self, parent):
        Gtk.EventBox.__init__(self)
        self.parent = parent
        self._tab = None
        self._sigtable = []

    def popola(self, w, h, wm):
        self.wm = wm
        self.chars = []
        self.rows = len(wm.schema)
        self.cols = len(wm.schema[0])
        for i in range(self.rows):
            for j in range(self.cols):
                c = wm.schema[i][j]
                cval = self.wm.cvalues[c.lower()]
                self.chars.append(RezCharNew(i, j, c, cval, None))

        self._tab = Gtk.DrawingArea()
        self.add(self._tab)

        self._tab.set_size_request(w, h)
        sigid = self._tab.connect("draw", self.on_draw)
        self._sigtable.append(sigid)
        sigid = self._tab.connect("configure-event", self.on_resize)
        self._sigtable.append(sigid)

        self.connect("button-press-event", self.button_press_cb)
        self.connect("button-release-event", self.button_release_cb)
        self.connect("motion-notify-event", self.motion_notify_cb)

    def spopola(self):
        for sigid in self._sigtable:
            self._tab.disconnect(sigid)
        self._sigtable = []
        self.remove(self._tab)
        self._tab = None

    def on_draw(self, obj, cairo_ctx):
        for c in self.chars:
            self._draw_char(c, cairo_ctx)

    def _draw_char(self, c, cr):
        margin = self._border
        #print cr.get_font_face().get_family()
        # family italic bold
        cr.select_font_face("Sans", 0, 1)
        cr.set_font_size((self.charh - 2*margin)*0.8)
        b_x, b_y, cwidth, cheight, ad_x, ad_y = cr.text_extents(c.char)
        border_x = (self.charw - cwidth)/2
        border_y = (self.charh - cheight)/2
        start_x = self.charw*c.col
        start_y = self.charh*c.row
        center_x = start_x + self.charw/2
        center_y = start_y + self.charh/2

        # disegna sfondo e linee di percorso se char attivo
        if c.active:
            # gr = Gtk.Gradient.new_radial(center_x, center_y, 0,
            #                              center_x, center_y,
            #                              min(self.charw, self.charh)/2)
            # start_c = Gtk.SymbolicColor.new_literal(Gdk.RGBA(0.1, 0.2, 0.6, 1))
            # stop_c = Gtk.SymbolicColor.new_literal(Gdk.RGBA(0.1, 0.2, 0.6, 0))
            # gr.add_color_stop(0, start_c)
            # gr.add_color_stop(1, stop_c)
            # cp = gr.resolve(Gtk.StyleProperties.new())
            # cr.set_source(cp)

            # rettangolo arrotondato
            rect_x = start_x + margin
            rect_y = start_y + margin
            rect_w = self.charw - 2*self._border
            rect_h = self.charh - 2*self._border
            aspect = 1.0
            corner_radius = rect_h / 10.0
            radius = corner_radius/aspect
            degrees = math.pi/180.0
            cr.new_sub_path()
            cr.arc(rect_x + rect_w - radius, rect_y + radius,
                   radius, -90*degrees, 0*degrees)
            cr.arc(rect_x + rect_w - radius, rect_y + rect_h - radius,
                   radius, 0*degrees, 90*degrees)
            cr.arc(rect_x + radius, rect_y + rect_h - radius,
                   radius, 90*degrees, 180*degrees)
            cr.arc(rect_x + radius, rect_y + radius,
                   radius, 180*degrees, 270*degrees)
            cr.close_path()
            cr.set_source_rgb(0.5, 0.5, 1)
            cr.fill_preserve()
            cr.set_source_rgba(0.5, 0, 0, 0.5)
            cr.set_line_width(4)
            cr.stroke()


            if c.arrow_x != 0 or c.arrow_y != 0:
                off = 0
                if abs(c.arrow_x*c.arrow_y) == 1:
                    # se ho angoli arrotondati, prolungo leggermente la linea
                    off = radius*0.4

                cr.move_to(center_x + (self.charw/2 - margin - off)*c.arrow_x,
                           center_y + (self.charh/2 - margin - off)*c.arrow_y)
                cr.set_line_width(10)
                cr.set_source_rgba(0.1, 0.2, 0.6, 0.5)
                cr.line_to(center_x + (self.charw/2 + margin + off)*c.arrow_x,
                           center_y + (self.charh/2 + margin + off)*c.arrow_y)
                cr.stroke()

        # # disegna cornice intorno a char, utile per centrare
        # cr.set_source_rgb(0, 0, 0)
        # cr.set_line_width(1.0)
        # cr.rectangle(start_x, start_y, self.charw, self.charh)
        # cr.stroke()

        # disegna char
        cr.set_source_rgba(0.1, 0.1, 0.1)
        # punta all'angolo in basso a sx del carattere
        # si muove a partire da angolo in alto a sx
        cr.move_to(start_x + border_x - b_x,
                   start_y + border_y - b_y)
        cr.show_text(c.char)

        # disegna punti del char
        cr.set_font_size((self.charh - 2*margin)*0.2)
        b_x, b_y, cwidth, cheight, ad_x, ad_y = cr.text_extents(str(c.value))
        cr.move_to(start_x + margin  + (self.charw - 2*margin)*0.8,
                   start_y + margin + cheight + (self.charh - 2*margin)*0.05)
        #cr.set_font_size(10)
        cr.show_text(str(c.value))

    def on_resize(self, obj, data):
        rect = obj.get_allocation()
        print 'resize', rect.width, rect.height
        self.charw = rect.width / self.cols
        self.charh = rect.height / self.rows
        self.gridw = rect.width
        self.gridh = rect.height

    def _get_char_by_cursor(self, ev):
        if ev.x < 0 or ev.x > self.gridw or ev.y < 0 or ev.y > self.gridh:
            return None

        # considera il contorno attorno alle lettere
        rel_x = ev.x % self.charw
        rel_y = ev.y % self.charh
        if rel_x < self._border or rel_x > (self.charw - self._border) or\
                rel_y < self._border or rel_y > (self.charh - self._border):
            return None

        col = int(ev.x / self.charw)
        row = int(ev.y / self.charh)
        for c in self.chars:
            if col == c.col and row == c.row:
                return c
        else:
            print 'char inesistente pos', tab_x, tab_y
            return None

    def handle_char_pressed(self, char):
        path = self.wm.put_char(char)
        for c in self.chars:
            if c not in path:
                c.active = False
        pre = cur = post = path[0]
        i = 0
        plen = len(path)
        while i < plen:
            if i < plen-1:
                post = path[i+1]
            cur.active = True
            # setto pre e post in arrow_x e arrow_y
            cur.arrow_x = pre.col - cur.col
            cur.arrow_y = pre.row - cur.row
            pre, cur = cur, post
            i += 1

        self.parent.update_word(''.join([c.char for c in path]))

    # FIXME: usa queue_draw_region sui soli caratteri da ridisegnare invece di
    # ridisegnare tutto

    def _char_active(self, event):
        char = self._get_char_by_cursor(event)
        if char:
            self.handle_char_pressed(char)
            self._tab.queue_draw()

    def motion_notify_cb(self, obj, event):
        # sembra che l'evento sia generato solo quando si clicca
        # in caso contrario dovrei installare la callback in caso di button_press
        #
        # continuo a raccogliere la sequenza di char
        self._char_active(event)

    def button_press_cb(self, tab, event):
        # inizio a raccogliere la sequenza di char
        self._char_active(event)

    def button_release_cb(self, tab, event):
        # fine raccolta sequenza char
        for child in self.chars:
            child.active = False

        self._tab.queue_draw()

        wchars = self.wm.stop_word()
        word = ''.join([c.char for c in wchars])
        if self.wm.check_word(word):
            print "HEHEHEHE"
        self.parent.update_word("")


class RezConfig:
    rows = 4
    columns = 4
    lang = 'ita'
    solve_all = False


class MainWin(Gtk.Window):
    # tmp. carica da file
    cfg = RezConfig
    def __init__(self, gm):
        Gtk.Window.__init__(self, title="Rezzlu")
        self.set_default_size(400, 400)

        box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        self.bbox = Gtk.Box(homogeneous=True)

        self.set_start_buttons()
        box.pack_start(self.bbox, False, False, 0)

        self.gm = gm
        self.tab = None

        self.add(box)
        self.tabbox = box

    def set_start_buttons(self):
        for c in self.bbox.get_children():
            self.bbox.remove(c)
            self.wordl = None

        startb = Gtk.Button(label="Start")
        startb.connect("clicked", self.on_start)
        self.bbox.pack_start(startb, True, False, 0)

        optb = Gtk.Button(label="Opzioni")
        optb.connect("clicked", self.on_options)
        self.bbox.pack_start(optb, True, False, 0)

    def set_run_buttons(self):
        for c in self.bbox.get_children():
            self.bbox.remove(c)

        stopb = Gtk.Button(label="Stop")
        stopb.connect("clicked", self.on_stop)
        self.bbox.pack_start(stopb, False, False, 0)

        self.wordl = Gtk.Label(label="")
        self.bbox.pack_start(self.wordl, True, True, 0)

        timer = self.gm.get_timer()
        timel = Gtk.Label(label="%s:%s" %(str(timer/60).zfill(2),
                                          str(timer%60).zfill(2)))
        timel.timer = timer
        def on_timer(timel):
            timel.timer -= 1
            m = timel.timer / 60
            s = timel.timer % 60
            timel.set_label("%s:%s" %(str(m).zfill(2), str(s).zfill(2)))
            if m == 0 and s == 0:
                self.on_timeout()
                return False
            else:
                return True
        GObject.timeout_add_seconds(1, on_timer, timel)
        self.bbox.pack_start(timel, True, True, 0)

    def update_word(self, word):
        self.wordl.set_label(word)
        #self.show_all()

    def show_results(self, wm):
        resw = Gtk.Window(Gtk.WindowType.TOPLEVEL, title="Rezzlu - Risultati")
        resw.set_default_size(600, 300)

        mainb = Gtk.VBox()
        mainb.pack_start(Gtk.Label(label="Punteggio: %d" %wm.score),
                         True, True, 0)

        resw.add(mainb)
        resw.show_all()
        return resw

    def on_timeout(self):
        #resw = self.show_results(self.tab.wm)
        self.on_stop()
        #resw.connect("delete-event", self.on_stop)

    def on_start(self, starttb):
        self.gm.do_config(self.cfg)

        prog = Gtk.Spinner()
        progl = Gtk.Label("Generazione gioco ...")
        self.tabbox.pack_start(prog, True, True, 0)
        self.tabbox.pack_end(progl, True, False, 0)
        prog.start()
        self.tabbox.show_all()

        def on_progress():
            while Gtk.events_pending():
                Gtk.main_iteration()

        wm = self.gm.get_wm(on_progress)

        prog.stop()
        self.tabbox.remove(prog)
        self.tabbox.remove(progl)

        #self.tab = RezTable()
        self.tab = RezTableNew(self)

        ww, wh = self.get_size()
        self.tab.popola(ww, wh - self.bbox.get_allocated_height(), wm)

        self.tabbox.pack_start(self.tab, False, False, 0)
        self.set_run_buttons()
        self.show_all()

    def on_options(self, optb):
        optw = Gtk.Window(Gtk.WindowType.TOPLEVEL, title="Rezzlu - Opzioni")
        optw.set_default_size(300, 100)
        hb = Gtk.HBox()
        vbleft = Gtk.VBox()
        vbright = Gtk.VBox()

        nrows = Gtk.Scale.new_with_range(Gtk.Orientation.HORIZONTAL, 2, 10, 1)
        nrows.set_value(self.cfg.rows)
        vbleft.pack_start(Gtk.Label("Numero di righe"), True, True, 0)
        vbright.pack_start(nrows, True, True, 0)

        ncols = Gtk.Scale.new_with_range(Gtk.Orientation.HORIZONTAL, 2, 10, 1)
        ncols.set_value(self.cfg.columns)
        vbleft.pack_start(Gtk.Label("Numero di colonne"), True, True, 0)
        vbright.pack_start(ncols, True, True, 0)

        cblang = Gtk.ComboBoxText()
        i = 0
        for i, lang in enumerate(['ita']):
            if lang == self.cfg.lang:
                cur_index = i
            cblang.append_text(lang)
        cblang.set_active(cur_index)
        vbleft.pack_start(Gtk.Label("Dizionario"), True, True, 0)
        vbright.pack_start(cblang, True, True, 0)

        solb = Gtk.CheckButton()
        solb.set_active(self.cfg.solve_all)
        vbleft.pack_start(Gtk.Label("Risoluzione automatica"), True, True, 0)
        vbright.pack_start(solb, True, True, 0)

        hb.pack_start(vbleft, True, True, 0)
        hb.pack_end(vbright, True, True, 0)
        optw.add(hb)

        def on_close(*a):
            self.cfg.rows = int(nrows.get_value())
            self.cfg.columns = int(ncols.get_value())
            self.cfg.lang = cblang.get_active_text()
            self.cfg.solve_all = solb.get_active()
            optw.destroy()

        optw.connect("delete-event", on_close)
        optw.show_all()

    def on_stop(self, stopb):
        self.show_results(self.tab.wm)
        self.set_start_buttons()

        self.tabbox.remove(self.tab)
        self.tab.spopola()
        self.tab.destroy()
        self.tab = None

        self.show_all()


def main_loop(gm):
    win = MainWin(gm)
    win.connect("delete-event", Gtk.main_quit)
    win.show_all()
    Gtk.main()
