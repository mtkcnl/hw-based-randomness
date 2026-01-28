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
                "AdditionalOptions": [ "/arch:AVX" ]
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