#https://stackoverflow.com/questions/10930386/alternative-for-random-module-in-python

class myrandom:
    kz=36969
    kw=18000
    k3=65535
    maxz=kz*k3+(2<<16)
    maxw=kw*k3+(2<<16)
    max=(maxz<<16 )+maxw
    # Optionally initiate with different seed. Two numbers below 2<<16
    def __init__(self,z=123456789,w=98764321):
        self.m_w = w
        self.m_z = z
    def step(self):
        self.m_z = self.kz * (self.m_z & self.k3) + (self.m_z >> 16)
        self.m_w = self.kw * (self.m_w & self.k3) + (self.m_w >> 16)
    def get(self):
        self.step()
        return (self.m_z << 16) + self.m_w
    def time_reseed(self):
        # yes, sure, move out import if you like to
        import time
        t=int(time.time())
        # completely made up way to got two new numbers below 2<<16
        self.m_z = (self.m_z+(t*34567891011)) & ((2<<16)-1)
        self.m_w = (self.m_w+(t*10987654321)) & ((2<<16)-1)
        self.step()
    def getuniform(self):
        return self.get()*1.0/self.max

myr=myrandom()

def shuffle(x, random=None):
        """x, random=random.random -> shuffle list x in place; return None.

        Optional arg random is a 0-argument function returning a random
        float in [0.0, 1.0); by default, the standard random.random.

        """

       # if random is None:
       #     random = self.random
        _int = int
        for i in reversed(xrange(1, len(x))):
            # pick an element in x[:i+1] with which to exchange x[i]
            j = int(myr.getuniform() * (i+1))
            x[i], x[j] = x[j], x[i]
            print j


def choice( seq):
        """Choose a random element from a non-empty sequence."""
        return seq[int(myr.getuniform() * len(seq))]  # raises IndexError if seq is empty



#NV_MAGICCONST = 4 * exp(-0.5)/sqrt(2.0)
#TWOPI = 2.0*pi
#LOG4 = log(4.0)
#SG_MAGICCONST = 1.0 + log(4.5)
BPF = 53        # Number of bits in a float
RECIP_BPF = 2**-BPF


def randrange( start, stop=None, step=1, _int=int, _maxwidth=1L<<BPF):
        """Choose a random item from range(start, stop[, step]).

        This fixes the problem with randint() which includes the
        endpoint; in Python this is usually not what you want.

        """

        # This code is a bit messy to make it fast for the
        # common case while still doing adequate error checking.
        istart = _int(start)
        if istart != start:
            raise ValueError, "non-integer arg 1 for randrange()"
        if stop is None:
            if istart > 0:
                if istart >= _maxwidth:
                    return self._randbelow(istart)
                return _int(myr.getuniform() * istart)
            raise ValueError, "empty range for randrange()"

        # stop argument supplied.
        istop = _int(stop)
        if istop != stop:
            raise ValueError, "non-integer stop for randrange()"
        width = istop - istart
        if step == 1 and width > 0:
            # Note that
            #     int(istart + self.random()*width)
            # instead would be incorrect.  For example, consider istart
            # = -2 and istop = 0.  Then the guts would be in
            # -2.0 to 0.0 exclusive on both ends (ignoring that random()
            # might return 0.0), and because int() truncates toward 0, the
            # final result would be -1 or 0 (instead of -2 or -1).
            #     istart + int(self.random()*width)
            # would also be incorrect, for a subtler reason:  the RHS
            # can return a long, and then randrange() would also return
            # a long, but we're supposed to return an int (for backward
            # compatibility).

            if width >= _maxwidth:
                return _int(istart + randbelow(width))
            return _int(istart + _int(myr.getuniform()*width))
        if step == 1:
            raise ValueError, "empty range for randrange() (%d,%d, %d)" % (istart, istop, width)

        # Non-unit step argument supplied.
        istep = _int(step)
        if istep != step:
            raise ValueError, "non-integer step for randrange()"
        if istep > 0:
            n = (width + istep - 1) // istep
        elif istep < 0:
            n = (width + istep + 1) // istep
        else:
            raise ValueError, "zero step for randrange()"

        if n <= 0:
            raise ValueError, "empty range for randrange()"

        if n >= _maxwidth:
            return istart + istep*self._randbelow(n)
        return istart + istep*_int(self.random() * n)




def randint( a, b):
        """Return random integer in range [a, b], including both end points.
        """

        return randrange(a, b+1)

#responses = ["Not so sure", "42", "Most likely", "Absolutely not", "Outlook is good", "I see good things happening", "Never",
#"Negative", "Could be", "Unclear, ask again", "Yes", "No", "Possible, but not probable"]

#myr.time_reseed();
#print randint(1,8);
#print(choice(responses))
#print randint(1,8);
#print(choice(responses))

#print randint(1,8);
#print(choice(responses))
