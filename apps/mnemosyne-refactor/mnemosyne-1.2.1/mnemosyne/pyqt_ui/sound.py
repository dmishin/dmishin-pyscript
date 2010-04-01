##############################################################################
#
# Sound support through pygame and SDL <Peter.Bienstman@UGent.be>
# OSX NSSound support by Konrad Wojas <k@wojas.nl>
#
##############################################################################

import sys, traceback

##############################################################################
#
# BaseSoundEngine
#
##############################################################################

class BaseSoundEngine(object):

    def __init__(self):
        raise NotImplementedError()

    def play(self, filepath):
        raise NotImplementedError()

    def is_playing(self):
        raise NotImplementedError()

    def update(self):
        raise NotImplementedError()


    
##############################################################################
#
# PyGameSoundEngine
#
#   Sound engine that uses the pygame package as backend.
#
##############################################################################

class PyGameSoundEngine(BaseSoundEngine):

    def __init__(self):

        import pygame
        self._mixer = pygame.mixer
        self._music = None
 

    def play(self, filepath):
            
        if not self._music:
            try:
                self._mixer.init()
                self._music = self._mixer.music
            except:
                print >> sys.stderr, "Unable to initialise sound."
                traceback.print_exc()
            
        if not self._music:
            return
        
        self._music.load(filepath)
        self._music.play()


    def is_playing(self):
        
        if not self._music:
            return False
        return self._music.get_busy()


    def update(self):

        if self._music and not self._music.get_busy():
            self._mixer.quit()
            self._music = None
            
            
##############################################################################
#
# OSXSoundEngine
#
# Sound engine that uses the PyObjC bindings on OSX to create an NSSound
# object.
#
#    Important to know about NSSound:
#    - The file can only be specified on initialisation.
#    - isPlaying can only be called after initialisation.
#    - To play another file you have to create a new instance.
#    - A sound can only be played once.
#
##############################################################################

class OSXSoundEngine(BaseSoundEngine):

    def __init__(self):

        from AppKit import NSSound
        self.NSSound = NSSound
        self._sound = None

    def _is_nil(self):
        return len(dir(self._sound))==0

    def play(self, filepath):

        # NSSound can be quite picky about how it is used, make sure it 
        # isn't playing anymore.
        
        if self.is_playing():
            return
        
        # I don't know of a better way to check if the object is NIL than
        # to check if dir(obj) is empty.
        
        if not self._sound is None and not self._is_nil():
            self._sound.dealloc()
            self._sound = None
        
        # Initialize and play.
        
        self._sound = self.NSSound.alloc()
        if self._is_nil():
            self._sound = None
            return
        self._sound.initWithContentsOfFile_byReference_(filepath, True)
        if self._is_nil():
            self._sound = None
            return
        self._sound.play()

    def is_playing(self):
        
        # isPlaying can only be called after initialisation.
        
        if self._sound is None or self._is_nil():
            return False
        else:
            return self._sound.isPlaying()

    def update(self):
        pass


##############################################################################
#
# SoundManager
#
#  Our backends only allow queuing a single sound at a time, so we write some
#  extra queuing code around it.
#  The update function needs to be called regularly, e.g. from a repeating
#  timer in the GUI.
#
##############################################################################

class SoundManager:

    def __init__(self, engine):

        self.engine = engine
        self.queue = []

    def play(self, filename):

        if self.engine.is_playing():
            self.queue.append(filename)
        else:
            self.engine.play(filename)

    def update(self):

        if len(self.queue) != 0 and not self.engine.is_playing():
            filename = self.queue.pop(0)
            self.engine.play(filename)

        self.engine.update()

        
if sys.platform == 'darwin':
    engine = OSXSoundEngine()
else:
    engine = PyGameSoundEngine()

soundmanager = SoundManager(engine)



##############################################################################
#
# play_sound
#
#  Extract path from a <sound src=".."> tag and play it.
#
##############################################################################

def play_sound(text):

    i = text.lower().find("sound src")

    while i != -1:

        start = text.find("\"", i)
        end   = text.find("\"", start+1)

        if end == -1:
            return

        filename = text[start+1:end]
        try:
            soundmanager.play(filename)
        except:
            print "Unable to play music from file", filename

        i = text.lower().find("sound src", i+1)



##############################################################################
#
# Commandline interface for testing this module: will play all files
# provided as arguments.
#
##############################################################################

if __name__ == '__main__':
    
    import time
    for fpath in sys.argv[1:]:
        soundmanager.play(fpath)
    while True:
        soundmanager.update()
        time.sleep(1)
        if not soundmanager.queue and not soundmanager.engine.is_playing():
            break
