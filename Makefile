ERLANG_PATH = $(shell erl -eval 'io:format("~s", [lists:concat([code:root_dir(), "/erts-", erlang:system_info(version), "/include"])])' -s init stop -noshell)

gl:
	gcc -fPIC -I$(ERLANG_PATH) -dynamiclib -undefined dynamic_lookup -o gl.so gl.c -framework GLUT -framework OpenGL

clean:
	rm -f gl.so Elixir.GL.beam

.PHONY: all clean
