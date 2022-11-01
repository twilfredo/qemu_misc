fn main() {
    let _build = cxx_build::bridge("src/lib.rs").compile("test_apps");

    println!("cargo:rerun-if-changed=src/lib.rs");
}
