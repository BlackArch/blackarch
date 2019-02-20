![malice logo](https://raw.githubusercontent.com/maliceio/malice/master/docs/images/logo/malice.png)

# malice

[![Circle CI](https://circleci.com/gh/maliceio/malice.png?style=shield)](https://circleci.com/gh/maliceio/malice) [![License](https://img.shields.io/badge/licence-Apache%202.0-blue.svg)](LICENSE) [![Release](https://img.shields.io/github/release/maliceio/malice.svg)](https://github.com/gmaliceio/malice/releases/latest) [![bh-arsenal](https://github.com/toolswatch/badges/blob/master/arsenal/usa/2018.svg)](https://www.blackhat.com/us-18/arsenal/schedule/index.html#maliceio-12000) [![Gitter](https://badges.gitter.im/maliceio/malice.svg)](https://gitter.im/maliceio/malice)

> Malice's mission is to be a free open source version of VirusTotal that anyone can use at any scale from an independent researcher to a fortune 500 company.

---

## Try It Out

> **DEMO:** [demo.malice.io](<https://demo.malice.io/app/kibana#/discover?_g=(refreshInterval:(pause:!t,value:0),time:(from:'2018-09-03T04:00:00.000Z',mode:absolute,to:'2018-09-10T04:00:00.000Z'))&_a=(columns:!(_source),index:afe16d30-b234-11e8-84d2-4fddc6da27ff,interval:auto,query:(language:lucene,query:''),sort:!(scan_date,desc))>)

- **username**: `malice`
- **password**: `ecilam`

## Requirements

### Hardware

- ~16GB disk space
- ~4GB RAM

### Software

- [Docker](https://docs.docker.com)

## Getting Started (OSX)

### Install

```bash
$ brew install maliceio/tap/malice
```

```
Usage: malice [OPTIONS] COMMAND [arg...]

Open Source Malware Analysis Framework

Version: 0.3.11

Author:
  blacktop - <https://github.com/blacktop>

Options:
  --debug, -D      Enable debug mode [$MALICE_DEBUG]
  --help, -h       show help
  --version, -v    print the version

Commands:
  scan        Scan a file
  watch        Watch a folder
  lookup    Look up a file hash
  elk        Start an ELK docker container
  plugin    List, Install or Remove Plugins
  help        Shows a list of commands or help for one command

Run 'malice COMMAND --help' for more information on a command.
```

### Scan some _malware_

```bash
$ malice scan evil.malware
```

> **NOTE:** On the first run malice will download all of it's default plugins which can take a while to complete.

Malice will output the results as a markdown table that can be piped or copied into a **results.md** that will look great on Github see [here](docs/examples/scan.md)

### Start Malice's Web UI

```bash
$ malice elk
```

> You can open the [Kibana](https://www.elastic.co/products/kibana) UI and look at the scan results here: <http://localhost> (_assuming you are using Docker for Mac_)

![kibana-setup](docs/images/kibana-setup.png)

- Type in **malice** as the `Index name or pattern` and click **Create**.

- Now click on the `Malice Tab` and **behold!!!**

![kibana-plugin](docs/images/new-screen.png)

## Getting Started (_Docker in Docker_)

[![CircleCI](https://circleci.com/gh/maliceio/malice.png?style=shield)](https://circleci.com/gh/maliceio/malice) [![Docker Stars](https://img.shields.io/docker/stars/malice/engine.svg)](https://hub.docker.com/r/malice/engine/) [![Docker Pulls](https://img.shields.io/docker/pulls/malice/engine.svg)](https://hub.docker.com/r/malice/engine/) [![Docker Image](https://img.shields.io/badge/docker%20image-30.6%20MB-blue.svg)](https://hub.docker.com/r/malice/engine/)

### Install/Update all Plugins

```bash
docker run --rm -v /var/run/docker.sock:/var/run/docker.sock malice/engine plugin update --all
```

### Scan a file

```bash
docker run --rm -v /var/run/docker.sock:/var/run/docker.sock \
                -v `pwd`:/malice/samples \
                -e MALICE_VT_API=$MALICE_VT_API \
                malice/engine scan SAMPLE
```

## Documentation

- [Documentation](docs)
- [Plugins](docs/plugins)
- [Examples](docs/examples)
- [Roadmap](docs/roadmap)
- [Contributing](CONTRIBUTING.md)

### Known Issues :warning:

#### If you are having issues with `malice` connecting/writting to `elasticsearch` please see the following:

I have noticed when running the new **5.0+** version of [malice/elasticsearch](https://github.com/maliceio/elasticsearch) on a linux host you need to increase the memory map areas with the following command

```bash
sudo sysctl -w vm.max_map_count=262144
```

Elasticsearch requires a **LOT** of RAM to run smoothly. You can lower it to **2GB** by running the following _(**before running a scan**)_:

```bash
$ docker run -d \
         -p 9200:9200 \
         -name malice-elastic \
         -e ES_JAVA_OPTS="-Xms2g -Xmx2g" \
         malice/elasticsearch
```

#### See here for more details on [Known Issues/FAQs](https://github.com/maliceio/malice/blob/master/docs/KnownBugs.md) :warning:

### Issues

Find a bug? Want more features? Find something missing in the documentation? Let me know! Please don't hesitate to [file an issue](https://github.com/maliceio/malice/issues/new)

### CHANGELOG

See [`CHANGELOG.md`](https://github.com/maliceio/malice/blob/master/CHANGELOG.md)

### License

Apache License (Version 2.0)<br>
Copyright (c) 2013 - 2018 **blacktop** Joshua Maine

 <!-- [![Slack](https://malice-slack.herokuapp.com/badge.svg)](https://malice-slack.herokuapp.com) -->
