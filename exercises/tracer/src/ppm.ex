defmodule PPM do

  # write(Name, Image) The image is a list of rows, each row a list of
  # tuples {R,G,B} where the values are flots from 0 to 1. The image
  # is written using PMM format P6 and color depth 0-255. This means that
  # each tuple is written as three bytes.

  def write(name, image) do
    height = length(image)
    width = length(List.first(image))
    {:ok, fd} = File.open(name, [:write, :binary])
    IO.puts(fd, "P6")
    IO.puts(fd, "#generated by ppm.ex")
    IO.puts(fd, "#{width} #{height}")
    IO.puts(fd, "255")
    rows(image, fd)
    File.close(fd)
  end

  defp rows(rows, fd) do
    Enum.each(rows, fn r ->
      colors = row(r)
      IO.write(fd, colors)
    end)
  end

  defp row(row) do
    List.foldr(row, [], fn({r, g, b}, a) ->
      [trunc(r * 255), trunc(g * 255), trunc(b * 255) | a]
    end)
  end

end
