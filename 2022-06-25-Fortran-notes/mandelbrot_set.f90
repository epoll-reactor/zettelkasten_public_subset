program main
  external compute_mandelbrot
  integer :: image_size
  integer :: gamma
  real    :: intensity
  real    :: background_distance
  real    :: scaling

  image_size = 100
  gamma = 255
  intensity = 100
  background_distance = 1.5
  scaling = 0.6
  
  call compute_mandelbrot(image_size, gamma, intensity, scaling, background_distance)
end

subroutine compute_mandelbrot_point(image_size, scaling, background_distance, intensity, x, y, output)
  real    :: background_distance
  real    :: intensity
  integer :: image_size
  real    :: output
  integer :: x, y
  complex :: z
  complex :: point
  real    :: iterations
  
  z     = cmplx(0.0, 0.0)
  point = cmplx((x / float(image_size)) - (1.55 / scaling), y / (float(image_size) - 0.5))

  do while (abs(z) < background_distance .and. iterations <= intensity)
    z = z * z + point
    z = z * scaling
    iterations = iterations + 1
  end do
  
  if (iterations < intensity) then
    output = (255 * intensity) / 40
  else
    output = 20.0
  end if
end subroutine compute_mandelbrot_point

subroutine compute_mandelbrot(image_size, gamma, intensity, scaling, background_distance)
  ! implicit none
  integer :: image_size
  integer :: gamma
  real    :: intensity
  real    :: scaling
  real    :: background_distance
  integer :: bytes_to_write
  real    :: r, g, b
  real    :: r_integer, g_integer, b_integer
  integer :: i, j
  real    :: computed_point
  
  ! character, allocatable, dimension(:) :: buffer
  
  bytes_to_write = image_size * image_size * 3
  ! allocate(buffer(bytes_to_write))

  call random_number(r)
  r_integer = floor(r * 4)
  call random_number(g)
  g_integer = floor(g * 4)
  call random_number(b)
  b_integer = floor(b * 4)

  open(1, file = '/home/krot/mandelbrot_set.png', status = 'new')

  write(1, 'P6 (A5) (A5)\n') 1, 1
  
  do i = 1, image_size
    do j = 1, image_size
      call compute_mandelbrot_point(image_size, scaling, background_distance, intensity, i, j, computed_point)
      
      ! print *, "el = ", i, " ", j, " ", computed_point
      ! buffer(image_size * j + i    ) = char(int(r_integer / computed_point))
      ! buffer(image_size * j + i + 1) = char(int(g_integer / computed_point))
      ! buffer(image_size * j + i + 2) = char(int(b_integer / computed_point))
      ! print *, "point = ", buffer(image_size * j + i    )
      ! print *, "point = ", buffer(image_size * j + i + 1)
      ! print *, "point = ", buffer(image_size * j + i + 2)
    end do
  end do
  ! deallocate(buffer)
end subroutine compute_mandelbrot
