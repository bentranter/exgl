EXGL
---

![image](https://cloud.githubusercontent.com/assets/5421627/12470929/abab85f6-bfc6-11e5-8c30-fb1f0f35e8d1.png)

An experiment to call OpenGl from Elixir.

Usage
---

1. Own a Mac (I haven't tried this on Windows or Linux yet, but I'm 100% sure the Makefile won't work on those platforms)
2. Don't be afraid of the 18 warnings Clang generates
3. Run `make`
4. Run Iex
5. In Iex, run `c "gl.ex"`
6. If that works, run `GL.start()`
7. Pray

Not Usage
---

Since the OpenGL code never returns, this literally halts the entire Erlang VM. I'm guessing there are two ways to deal with this: create a pthread from C and run the OpenGL code in that so the NIF function can actually return, or better, spawn a process in Elixir and call this NIF func??? I'm not sure yet, will update in a future commit if I ever make one _(note: there's no way I'm going to work on this again)_.
