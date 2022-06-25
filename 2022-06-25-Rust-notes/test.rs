 fn main() {
     let number = 12;
     match number {
         1 => println!("One"),
         2 | 3 | 5 | 7 | 11 => println!("Prime number"),
         13..=19 => println!("13..=19"),
         _ => println!("Ain't special")
   }
     let boolean = false;
     let binary = match boolean {
         false => 0,
         true => 1,
     };

     println!("{} -> {}", boolean, binary);
 }

 use std::io;

 fn main() {
  println!("Guess the number");
  println!("Please input value");
  let mut guess = String::new();
  io::stdin().read_line(&mut guess)
    .expect("Failed to read line");
  println!("You guessed: {}", guess);
 }

 macro_rules! init_macro {
  ($function:ident) => (
    fn $function() {
      println!("{:?} function called",
        stringify!($function))
    }
  )
 }

 //-> macro
 init_macro!(foo);

 macro_rules! print_result {
  ($expression:expr) => (
    println!("{:?} = {:?}",
      stringify!($expression), $expression);
  )
 }

 fn main() {
  foo();
  print_result!(1u32 + 1);
  print_result!({
         let x = 1u32;
     });
 }

//< check if linux
 #[cfg(target_os = "linux")]
 fn if_linux() {
  println!("Code running in Linux.");
 }

 #[cfg(not(target_os = "linux"))]
 fn if_linux() {
  println!("Code running in non-linux system.");
 }

 fn main() {
  if_linux();
 }

 use std::thread;

 fn main() {
  let data = "86967897737416471853297327050364959
        11861322575564723963297542624962850
        70856234701860851907960690014725639
        38397966707106094172783238747669219
        52380795257888236525459303330302837
        58495327135744041048897885734297812
        69920216438980873548808413720956532
        16278424637452589860345374828574668";
  let mut children = vec![];
  let chunked_data = data.split_whitespace();
  for (i, data_segment) in chunked_data.enumerate() {
    println!("data segment {} is \"{}\"", i, data_segment);
  children.push(thread::spawn(move || -> u32 {
    let result = data_segment
      .chars()
      .map(|c| c.to_digit(10).expect("should be a digit"))
      .sum();
    println!("processed segment {}, result={}", i, result);
    result
  }));
  }

  let mut intermediate_sums = vec![];
  for child in children {
    let intermediate_sum = child.join().unwrap();
    intermediate_sums.push(intermediate_sum);
  }
  let final_result = intermediate_sums.iter().sum::<u32>();
  println!("Final sum result: {}", final_result);
 }


 fn main() {
  let mut array: [i32; 3] = [0; 3];
  array[1] = 1;
  array[2] = 2;
  assert_eq!([1, 2], &array[1..]);
  for x in array.iter() {
    print!("{} ", x);
  }
 }

 use std::alloc::{GlobalAlloc, System, Layout};

 struct MyAllocator;

 unsafe impl GlobalAlloc for MyAllocator {
  unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
    System.alloc(layout)
  }
  unsafe fn dealloc(&self, ptr: *mut u8, layout: Layout) {
    System.dealloc(ptr, layout)
  }
 }

 #[global_allocator]
 static GLOBAL: MyAllocator = MyAllocator;

 fn main() {
  let mut v = Vec::new();
  v.push(1);
 }
