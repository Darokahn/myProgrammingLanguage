No time for a full writeup. Modules will be handled via an extension of classical linking practices. The key innovation is emission of headerlike object files rather than text inclusion. Simple libraries can be compiled directly into a header object, no need to maintain a headerfile. The ELF and the header will be emitted together into a module file. There some important details that need resolution. I will write down notes for those details here and hope that I remember what I meant later.

include vs. link vs. import (include - parse a declaration header. link - satisfy declarations using the contents of an ELF. import - parse a module file).

Enforcement headers

Compilation resolution order

