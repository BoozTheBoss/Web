import adapter from '@sveltejs/adapter-node';

/** @type {import('@sveltejs/kit').Config} */
const config = {
  kit: {
    adapter: adapter({
      out: 'build',
      precompress: false,
      envPrefix: '',
      polyfill: true
    }),
    csrf: {
      checkOrigin: false,
    }
  }
};

export default config;
