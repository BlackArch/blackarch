# Docker Images

## Introduction

This directory includes everything we need to build a Docker image, which is then used to build the actual BlackArch image. This way, we avoid cluttering our environment with build artifacts.

## Requirements

We need a working local Docker client. It is irrelevant to which Docker [engine](https://docs.docker.com/engine/#about-docker-engine) (daemon) the client is connected, though in most cases the engine will be local as well.

## Usage

First, we will build an `blackarch-builder` image including everything we need to build the actual `blackarch` image. The following command will do that for you.

    docker build -t blackarch-builder .

We now have the `blackarch-builder` image in our Docker engine and start a container based on that image. We mount the Docker control socket which the builder script will need to import the finished image into the Docker engine. The `--privileged` flag is necessary to be able to `chroot` in the container.

    docker run --privileged -v /var/run/docker.sock:/var/run/docker.sock blackarch-builder

We could optionally add `-v $PWD/output:/output` to also mount the build output directory so we can grab the raw Docker image after it has been created. In most cases, however, having the finished image imported in the Docker engine will suffice.

The [entrypoint](https://docs.docker.com/engine/reference/builder/#entrypoint) of the `blackarch-builder` image will [pacstrap](https://wiki.archlinux.org/index.php/Install_from_existing_Linux) a minimal Arch Linux installation to a temporary directory under `/output`, perform various configuration steps on that installation (some via a script injected into the chroot, because of some limitations of `arch-bootstrap`), install the BlackArch keys and repositories and finally tar the directory and import that tarball via the Docker control socket into the Docker engine behind the socket.

The last line of output should be `Success.`, which comes from an `echo` inside a container based on our finished `blackarch` image. Now we can start BlackArch containers.

    docker run -ti blackarch bash