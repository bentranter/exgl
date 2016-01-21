# Our module name
defmodule GL do
  # When the module loads, call the initialization functions below
  @on_load :init

  # Execute the Erlang code to load the shared object library that contains our
  # NIF
  def init() do
    :erlang.load_nif("gl", 0)
  end

  # This is just a check to make sure our function exists, and that the
  # initialization worked
  def start() do
    raise "NIF binding/0 not implemented!"
  end

  # HAHA
  def printscr(_str) do
    raise "Honestly it's better you don't call this function..."
  end
end
