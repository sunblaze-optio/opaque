#!/bin/bash
cd src/enclave/Enclave/dpml-rust/
cargo build
cd ../../../../
build/sbt package
spark-shell --jars ${OPAQUE_HOME}/target/scala-2.11/opaque_2.11-0.1.jar
