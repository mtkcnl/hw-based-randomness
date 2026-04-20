{
  "targets": [
    {
      "target_name": "hw_rng",
      "sources": [ "main.c" ],
      "conditions": [
        [
          "OS=='win'",
          {
            "msvs_settings": {
              "VCCLCompilerTool": {
                "AdditionalOptions": [ "/O2", "/arch:AVX512" ]
              }
            }
          },
          {
            "cflags": [ "-O3", "-mrdrnd", "-mrdseed" ],
            "xcode_settings": {
              "OTHER_CFLAGS": [ "-O3", "-mrdrnd", "-mrdseed" ]
            }
          }
        ]
      ]
    }
  ]
}