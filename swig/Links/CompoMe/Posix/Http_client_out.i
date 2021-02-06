/* /\* Http_client_out.i *\/ */

%module Http_client_out;
%include <std_string.i>

%include "Interfaces/Interface.i"


%include "Links/Link.i"






%include "Types/CompoMe/String.i"



%include "Types/i32.i"




%module Http_client_out
%{
#include "Links/CompoMe/Posix/Http_client_out/Http_client_out.hpp"
%}

%include "Links/CompoMe/Posix/Http_client_out/Http_client_out.hpp"