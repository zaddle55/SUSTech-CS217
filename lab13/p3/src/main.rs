use std::rc::Rc;
struct DangerousContainer {
    // data: *mut i32,
    data: Rc<i32>,
}

impl DangerousContainer {
    fn new(value: i32) -> Self {
        // let ptr = Box::into_raw(Box::new(value)); //? raw ptr needs to be dropped
        DangerousContainer { data: Rc::new(value) }
    }

    fn create_dangling() -> *mut i32 {
        // let local_data = 42; //? dangling reference
        // &local_data as *const i32 as *mut i32 
        let local_data = Box::new(42); // safe dangling reference
        Box::into_raw(local_data)
    }

    // unsafe fn get(&self) -> i32 {
    //     *self.data
    // }
    fn get(&self) -> i32 {
        *self.data
    }

    fn clone(&self) -> DangerousContainer {
        DangerousContainer {
            data: Rc::clone(&self.data),
        }
    }

    fn rc(&self) -> usize {
        Rc::strong_count(&self.data)
    }
}

fn main() {
   
    let container1 = DangerousContainer::new(10);
    let dangling_ptr = DangerousContainer::create_dangling();
    unsafe {
        println!("Dangling value: {}", *dangling_ptr);
    }

    unsafe {dangling_ptr.drop_in_place();}

    // let ptr = Box::into_raw(Box::new(20));
    let ptr = Rc::new(20);
    let container2 = DangerousContainer { data: ptr };
    // let container3 = DangerousContainer { data: ptr };
    let container3 = container2.clone();
    println!("Container1 value: {}", container1.get());
    println!("Container2 value: {}", container2.get());
    println!("Container3 value: {}", container3.get());
    println!("Container2 rc: {}", container2.rc());
    println!("Container3 rc: {}", container3.rc());

    {
        let container4 = container2.clone();
        println!("Container4 value: {}", container4.get());
        println!("Container2 rc: {}", container2.rc());
        println!("Container4 rc: {}", container4.rc());
    }
    // drop(container4);

    println!("Container2 rc: {}", container2.rc());
    
    // drop(container2);
    // drop(container3);
}

