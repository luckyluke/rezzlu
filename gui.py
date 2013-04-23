import os

from gi.repository import Gtk, Gdk, GObject
from gi.repository.GdkPixbuf import Pixbuf, InterpType
#from gi import _glib
from gi._glib import GError

class RezChar(Gtk.Image):
    def __init__(self, char, row, col, w, h):
        """
        row\col 0 1 2 3
           0    x x x x
           1    x x x x
           2    x x x x
           3    x x x x
        """
        Gtk.Image.__init__(self)
        self.row = row
        self.col = col
        self.char = char
        self._active = None
        self._pre = None
        self._post = None
        self._width = w
        self._height = h

    def set_active(self, active, pre=None, post=None):
        if self._active == active and self._pre == pre and self._post == post:
            return
        if active:
            fname = "images/cell_%s_active.svg" %self.char
            self._pre = pre
            self._post = post
        else:
            fname = "images/cell_%s.svg" %self.char
            self._pre = self._post = None

        pre_arrow = self.get_super_fnames(pre)
        post_arrow = self.get_super_fnames(post)
        try:
            pix = Pixbuf.new_from_file_at_size(fname, self._width, self._height)
            if pre_arrow:
                pix_pre = Pixbuf.new_from_file_at_size(pre_arrow,
                                                       self._width, self._height)
                pix_pre.composite(pix, 0, 0, pix.props.width, pix.props.height,
                                  0, 0, 1.0, 1.0, InterpType.HYPER, 255)
            if post_arrow:
                pix_post = Pixbuf.new_from_file_at_size(post_arrow,
                                                        self._width, self._height)
                pix_post.composite(pix, 0, 0, pix.props.width, pix.props.height,
                                   0, 0, 1.0, 1.0, InterpType.HYPER, 255)
                
            self.set_from_pixbuf(pix)
            self._active = active
        except GError, e:
            print 'error', e

    def get_super_fnames(self, char):
        """
        Coordinate per la freccia:

        (-1, -1) (-1, 0) (-1, 1)

        (0, -1)           (0, 1)

        (1, -1)   (1, 0)  (1, 1)

        """
        if char:
            arrow_x, arrow_y = char.row - self.row, char.col -  self.col
            if (arrow_x, arrow_y) != (0, 0):
                fname = "images/arrow_%d_%d.svg" %(arrow_x, arrow_y)
                if os.path.exists(fname):
                    return fname
        return None

    def __str__(self):
        return "%s at (%d, %d)" %(self.char, self.row, self.col)

class RezTable(Gtk.EventBox):
    _border = 10
    def __init__(self):
        Gtk.EventBox.__init__(self)
        self.set_above_child(True)
        self._sigtable = []

    def popola(self, w, h, wm):
        charmatrix = wm.schema
        w = w / len(charmatrix)
        h = h / len(charmatrix[0])
        charw = w - self._border
        charh = h - self._border
        if charw < 1 or charh < 1:
            charw = charh = 75

        self._tab = Gtk.Table(wm.rows, wm.cols, True)
        for i in range(len(charmatrix)):
            for j in range(len(charmatrix[i])):
                b = RezChar(charmatrix[i][j], i, j, charw, charh)
                b.set_active(False)
                self._tab.attach(b, j, j+1, i, i+1,
                                 Gtk.AttachOptions.EXPAND,
                                 Gtk.AttachOptions.EXPAND)
                #b.set_property("expand", True)
                #self._tab.attach(b, j, i, 1, 1)

        sigid = self.connect("button-press-event", self.button_press_cb)
        self._sigtable.append(sigid)
        sigid = self.connect("button-release-event", self.button_release_cb)
        self._sigtable.append(sigid)
        sigid = self.connect("motion-notify-event", self.motion_notify_cb)
        self._sigtable.append(sigid)

        self.add(self._tab)
        self.wm = wm

    def spopola(self):
        for sigid in self._sigtable:
            self.disconnect(sigid)
        self._sigtable = []
        self.remove(self._tab)

    def _get_char_by_cursor(self, ev):
        rect = self._tab.get_allocation()
        rows, columns = self._tab.get_size()
        if ev.x < 0 or ev.x > rect.width or ev.y < 0 or ev.y > rect.height:
            #print "out of range"
            return None

        char_w = rect.width / rows
        char_h = rect.height / columns

        # considera il contorno attorno alle lettere
        rel_x = ev.x % char_w
        rel_y = ev.y % char_h
        if rel_x < self._border or rel_x > (char_w - self._border) or\
                rel_y < self._border or rel_y > (char_h - self._border):
            return None
        col = int(ev.x / char_w)
        row = int(ev.y / char_h)
        for child in self._tab.get_children():
            if child.row == row and child.col == col:
                return child
        else:
            print 'char inesistente pos', tab_x, tab_y
            return None

    def handle_char_pressed(self, char):
        path = self.wm.put_char(char)
        for c in self._tab.get_children():
            if c not in path:
                c.set_active(False)
        pre = cur = post = path[0]
        i = 0
        plen = len(path)
        while i < plen:
            if i < plen-1:
                post = path[i+1]
            cur.set_active(True, pre, post)
            pre, cur = cur, post
            i += 1

    def motion_notify_cb(self, obj, event):
        # sembra che l'evento sia generato solo quando si clicca
        # in caso contrario dovrei installare la callback in caso di button_press
        char = self._get_char_by_cursor(event)
        if char:
            # continuo a raccogliere la sequenza di char
            self.handle_char_pressed(char)

    def button_press_cb(self, tab, event):
        # inizio a raccogliere la sequenza di char
        char = self._get_char_by_cursor(event)
        if char:
            self.handle_char_pressed(char)

    def button_release_cb(self, tab, event):
        # fine raccolta sequenza char
        for child in self._tab.get_children():
            child.set_active(False)

        wchars = self.wm.stop_word()
        word = ''.join([c.char for c in wchars])
        if self.wm.check_word(word):
            print "HEHEHEHE"

class RezConfig:
    rows = 4
    columns = 4
    lang = 'ita'
    # TODO
    solve_all = True


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
        self.tab = RezTable()

        self.add(box)
        self.tabbox = box

        #w, h = self.get_default_size()
        #self.tab.popola(w, h)
        #_glib.idle_add(self.tab.popola, TEST)

    def set_start_buttons(self):
        for c in self.bbox.get_children():
            self.bbox.remove(c)

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

    def on_timeout(self):
        print "timeout!"

    def on_start(self, starttb):
        self.gm.do_config(self.cfg)
        self.set_run_buttons()

        wm = self.gm.get_wm()

        w, h = self.get_size()
        self.tab.popola(w, h, wm)

        self.tabbox.pack_start(self.tab, False, False, 0)
        self.show_all()

    def on_options(self, optb):
        pass

    def on_stop(self, stopb):
        self.set_start_buttons()

        self.tabbox.remove(self.tab)
        self.tab.spopola()

        self.show_all()

    #def show(self):
    #    Gtk.Window.show(self)
    #    self.tab.popola(TEST)
    #    GObject.idle_add(self.tab.popola, TEST)
    #    Gdk.threads_add_idle(_glib.PRIORITY_DEFAULT_IDLE, self.tab.popola, TEST)


def main_loop(gm):
    #Gdk.threads_init()
    win = MainWin(gm)
    win.connect("delete-event", Gtk.main_quit)
    win.show_all()
    Gtk.main()
